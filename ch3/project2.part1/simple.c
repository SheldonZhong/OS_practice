#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

struct task_struct *task;

/* This function is called when the module is loaded. */

int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       printk(KERN_INFO "state\tpid\tname\n");
       for_each_process(task)
       {
              printk(KERN_INFO "%ld\t%d\t%s\n", task->state, task->pid, task->comm);
       }

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
       printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
