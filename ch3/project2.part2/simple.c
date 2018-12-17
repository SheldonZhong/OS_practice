#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

struct task_struct *task;
struct list_head *list;

void DFS(struct list_head *list, struct list_head *next)
{
       list_for_each(list, next)
       {
              task = list_entry(list, struct task_struct, sibling);
              printk(KERN_INFO "%ld\t%d\t%s\n", task->state, task->pid, task->comm);
              DFS(list, &task->children);
              // task points to the next child in the list
       }
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       printk(KERN_INFO "state\tpid\tname\n");
       // 1, 8415, 8416, 9298, 9204, 2, 6, 200, 3028, 3610, 4005

       DFS(list, &init_task.children);
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
