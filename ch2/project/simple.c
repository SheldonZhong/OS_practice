#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
static LIST_HEAD(birthday_list);

struct birthday
{
       int day;
       int month;
       int year;
       struct list_head list;
};

void set_birthday(struct birthday *person, int day, int month, int year);

/* This function is called when the module is loaded. */
int simple_init(void)
{
       struct birthday *peter, *tom, *jhon, *mike, *sheldon;
       struct birthday *ptr;
       printk(KERN_INFO "Loading Module\n");

       peter = kmalloc(sizeof(*peter), GFP_KERNEL);
       tom = kmalloc(sizeof(*tom), GFP_KERNEL);
       jhon = kmalloc(sizeof(*jhon), GFP_KERNEL);
       mike = kmalloc(sizeof(*mike), GFP_KERNEL);
       sheldon = kmalloc(sizeof(*sheldon), GFP_KERNEL);

       set_birthday(peter, 12, 3, 2000);
       set_birthday(tom, 29, 5, 1997);
       set_birthday(jhon, 1, 1, 1995);
       set_birthday(mike, 2, 3, 1990);
       set_birthday(sheldon, 4, 4, 1997);

       list_for_each_entry(ptr, &birthday_list, list)
       {
              printk(KERN_INFO "person day: %d, month: %d, year: %d \n", ptr->day, ptr->month, ptr->year);
       }

       return 0;
}

void set_birthday(struct birthday *person, int day, int month, int year)
{
       person->day = day;
       person->month = month;
       person->year = year;
       INIT_LIST_HEAD(&person->list);
       list_add_tail(&person->list, &birthday_list);
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
       struct birthday *ptr, *next;
       printk(KERN_INFO "Removing Module\n");

       list_for_each_entry_safe(ptr, next, &birthday_list, list)
       {
              printk(KERN_INFO "Removing day: %d, month: %d, year: %d \n", ptr->day, ptr->month, ptr->year);
              list_del(&ptr->list);
              kfree(ptr);
       }
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
