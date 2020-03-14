#include "sched.h"
#include <linux/slab.h>
#include <linux/irq_work.h>

static DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

void init_iot_rq(struct iot_rq *iot_rq)
{
    struct iot_prio_array *array;
    int i;

    // get the address of the active run-queues
    array = &iot_rq->active;

    // init the headers of each run-queues with different priority
    for (i = 0; i < MAX_IOT_PRIO; i++) {
        INIT_LIST_HEAD(array->queue + i);
        __clear_bit(i, array->bitmap);
    }

    // set the bitmap for the sepcific CPU in order to find the queues with different priority in multiple CPUs.
    __set_bit(MAX_IOT_PRIO, array->bitmap);

    // get value of the highest priority
    iot_rq->highest_prio.curr = MAX_IOT_PRIO;

    /* We start is dequeued state, because no RT tasks are queued */
    iot_rq->iot_queued = 0;
    iot_rq->overloaded = 0;
    iot_rq->iot_throttled = 0;
}

/*
 * TODO: This function is called when a task is ready to be
 * put on the runqueue. The flags specify the reason
 * the function has been called.
 */
static void enqueue_task_iot(struct rq *rq, struct task_struct *p, int flags)
{
    return;
}

/*
 * TODO: dequeue_task are used to remove a task from the run queue,
 */
static void dequeue_task_iot(struct rq *rq, struct task_struct *p, int flags)
{
    return;
}

/*
 * TODO: In this function, you will be choosing which task will be run next.
 * Which task you choose will depend on how you organize your tasks
 * in the runqueue. Remember that there is a per-CPU runqueue. This
 * function is called primarily from the core schedule() function.
 *
 * You can look up 'struct rq_flags' for the flags that are passed.
 */

static struct task_struct *pick_next_task_iot(struct rq *rq,
                        struct task_struct *prev, struct rq_flags *rf)
{
    return NULL;
}


/*
 * In this function, you will choose which CPU the task is assigned to.
 * This can be used to distribute your tasks to multiple CPUs, as there
 * is a per-CPU runqueue.
 */

static int select_task_rq_iot(struct task_struct *p, int cpu,
                                    int sd_flag, int flags)
{
    return cpu;
}

/*
 * This function puts the runqueue online, if it is not already. The
 * function is called:
 *  1) In the early boot process
 *  2) At runtime, if cpuset_cpu_active() fails to rebuild the domains
 *
 * In basic terms: A cpuset is a mechanism that allows one to assign a 
 * set of CPUs and Memory to a set of tasks.
 *
 * You can use this function to maintain stats or set masks.
 * i.e. CPU priority, mask, etc.
 */

static void rq_online_iot(struct rq *rq)
{
    cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/*
 * This function is called in the event the runqueue is being taken offline.
 * You can free up any resources belonging to the specific CPU runqueue being
 * passed. i.e. any stats or masks being held for the CPU or runqueue
 */

static void rq_offline_iot(struct rq *rq)
{
    cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}


/*
 * This function is called to initialize your scheduler class. This will only
 * be called once, not per-CPU. Here you can initialize any variables, masks,
 * priorities, etc.
 */

void __init init_sched_iot_class(void)
{
    unsigned int i;
    for_each_possible_cpu(i) {
        zalloc_cpumask_var_node(&per_cpu(local_cpu_mask, i),GFP_KERNEL, cpu_to_node(i));
    }
}


/*
 * unused functions of the scheduler
 */

static void task_woken_iot(struct rq *rq, struct task_struct *p){}
static void set_curr_task_iot(struct rq *rq) {}
static void update_curr_iot(struct rq *rq) {}
static void switched_from_iot(struct rq *rq, struct task_struct *p){}
static void prio_changed_iot(struct rq *rq, struct task_struct *p, int oldprio) {}
static void task_tick_iot(struct rq *rq, struct task_struct *p, int queued) {}
static void switched_to_iot(struct rq *rq, struct task_struct *p) {}
static void put_prev_task_iot(struct rq *rq, struct task_struct *p) {}
static void yield_task_iot(struct rq *rq) {}
static void check_preempt_curr_iot(struct rq *rq, struct task_struct *p, int flags) {}

const struct sched_class iot_sched_class = {
        .next			    = &fair_sched_class,
        .enqueue_task	    = enqueue_task_iot,
        .dequeue_task	    = dequeue_task_iot,
        .yield_task		    = yield_task_iot,
        .check_preempt_curr	= check_preempt_curr_iot,
        .pick_next_task		= pick_next_task_iot,
        .put_prev_task		= put_prev_task_iot,
        .select_task_rq		= select_task_rq_iot,
        .set_cpus_allowed   = set_cpus_allowed_common,
        .rq_online          = rq_online_iot,
        .rq_offline         = rq_offline_iot,
        .task_woken		    = task_woken_iot,
        .switched_from		= switched_from_iot,
        .set_curr_task      = set_curr_task_iot,
        .task_tick		    = task_tick_iot,
        .prio_changed		= prio_changed_iot,
        .switched_to		= switched_to_iot,
        .update_curr		= update_curr_iot,
};
