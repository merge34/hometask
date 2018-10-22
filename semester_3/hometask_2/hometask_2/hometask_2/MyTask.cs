﻿namespace CustomThreading
{
    using System;
    using System.Threading;

    /// <summary>
    /// Represents task generated by <see cref="MyThreadPool"/>
    /// </summary>
    /// <typeparam name="TResult">Task result type</typeparam>
    public class MyTask<TResult> : IMyTask<TResult>
    {
        /// <summary>
        /// Function which is being calculated by <see cref="taskThread"/>
        /// </summary>
        private readonly Func<TResult> supplier;

        /// <summary>
        /// Pool that created current task
        /// </summary>
        private MyThreadPool parentPool;

        /// <summary>
        /// Blocking thread that called Result until task is not executed
        /// </summary>
        private ManualResetEvent resultGuard = new ManualResetEvent(false);

        /// <summary>
        /// Calculation result
        /// </summary>
        private TResult taskResult;

        /// <summary>
        /// Exception which is possibly occurred during calculation
        /// </summary>
        private Exception occuredException = null;

        /// <summary>
        /// Initializes a new instance of the <see cref="MyTask{TResult}"/> class.
        /// </summary>
        /// <param name="parentPool">Pool that created these task</param>
        /// <param name="supplier">Function to calculate</param>
        public MyTask(MyThreadPool parentPool, Func<TResult> supplier)
        {
            this.parentPool = parentPool;
            this.supplier = supplier;
        }

        /// <summary>
        /// Gets a value indicating whether task is completed
        /// </summary>
        public bool IsCompleted { get; private set; } = false;

        /// <summary>
        /// Gets task result. Blocks caller thread until result is ready
        /// </summary>
        public TResult Result
        {
            get
            {
                // Waiting while result is being calculated
                this.resultGuard.WaitOne();

                if (this.occuredException == null)
                {
                    return this.taskResult;
                }
                else
                {
                    throw new AggregateException(this.occuredException);
                }
            }
        }

        /// <summary>
        /// Generates new task based on the result of current task
        /// </summary>
        /// <typeparam name="TNewResult">New task result type</typeparam>
        /// <param name="supplier">New supplier function</param>
        /// <returns>
        /// Task which executes new supplier function
        /// with the result of current task as a parameter
        /// </returns>
        public IMyTask<TNewResult> ContinueWith<TNewResult>(
            Func<TResult, TNewResult> supplier)
        {
            TNewResult supplierWrapper() => supplier(this.Result);

            return this.parentPool.AddTask(supplierWrapper);
        }

        /// <summary>
        /// Executes task synchronously
        /// </summary>
        /// <param name="preventExecution">
        /// Mark task as not executable
        /// (Result will throw exception)</param>
        public void ExecuteTaskManually(bool preventExecution = false)
        {
            if (!preventExecution)
            {
                try
                {
                    this.taskResult = this.supplier();
                }
                catch (Exception e)
                {
                    this.occuredException = e;
                }
            }
            else
            {
                this.occuredException =
                    new AggregateException("Task execution cancelled!");
            }

            this.IsCompleted = true;
            this.resultGuard.Set();
        }
    }
}