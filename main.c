#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/err.h>
#include <linux/sched.h>

#include <linux/mailbox.h>

MODULE_AUTHOR("Kevin Hilman");
MODULE_LICENSE("GPL");

/* load-time options */
int count = 16;
char *name = "mbox-ipu1";

module_param(count, int, 0);
module_param(name, charp, 0);

static int callback(struct notifier_block *this, unsigned long index,
			void *data)
{
	int ret = 0;
	struct mailbox_msg *msg = data;
	u32 msg_data = (u32)msg->pdata;

	pr_info("mbox msg: 0x%x\n", msg_data);

	return ret;
}

struct notifier_block nb = {
	.notifier_call = callback,
};

static struct mailbox *mbox;

static void __exit mbox_test_cleanup(void)
{
	pr_info("%s: mbox_test_cleanup entered\n", __func__);
	if (mbox)
		mailbox_put(mbox, &nb);
}

static int __init mbox_test_init(void)
{
	int i, r, ret = 0;
	struct mailbox_msg msg;

	pr_info("%s: mbox_test_init entered\n", __func__);
	mbox = mailbox_get(name, &nb);
	if (IS_ERR(mbox)) {
		pr_err("%s: mailbox_get() failed: 0x%p\n", __func__, mbox);
		mbox = NULL;
		ret = -EINVAL;
		goto out;
	}

	for (i = 0; i < count; i++) {

		MAILBOX_FILL_MSG(msg, 0, i, 0);
		r = mailbox_msg_send(mbox, &msg);
		if (r) {
			pr_err("%s: mailbox_msg_send() failed: %d\n",
			       __func__, r);
			/* Let callback empty fifo a bit, then continue: */
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout(HZ / 10);  /* 1/10 second */
			i--;
			continue;
		}
	}

out:
	return ret;
}

module_init(mbox_test_init);
module_exit(mbox_test_cleanup);
