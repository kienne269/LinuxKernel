#include<linux/init.h>
#include<linux/module.h>

int GiaiThua(int n);
void InMaTran(int n);

static int N = 100;

module_param(N, int, S_IRUGO);

static int p02_init(void)
{
    	printk(KERN_ALERT "Khoi tao ok\n");
        
        printk(KERN_INFO "\nGiai thua %d! = %d",N, GiaiThua(N));

        return 0;
}

int GiaiThua(int n)
{
        int gt = 2;
        int i = 3;

        for(i=3; i<=n; i++)
        {
                gt *= i;
        }

        return gt;
}


static void p02_exit(void)
{
    printk(KERN_ALERT "Ket thuc ok\n");

}

module_init(p02_init);
module_exit(p02_exit);

MODULE_LICENSE("GPL");