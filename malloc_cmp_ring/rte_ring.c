#include <rte_ring.h>
#include <assert.h>

struct rte_ring *rte_ring_create(const char *name, unsigned count, unsigned flags)
{
	struct rte_ring *r = NULL;

	r = malloc(sizeof(*r) + sizeof(void *) * count);
	assert(r);

	memset(r, 0, sizeof(*r) + sizeof(void *) * count);
	r->name = name;
    r->prod.watermark = count;
    r->prod.sp_enqueue = !!(flags & RING_F_SP_ENQ);
    r->cons.sc_dequeue = !!(flags & RING_F_SC_DEQ);
    r->prod.size = r->cons.size = count;
    r->prod.mask = r->cons.mask = count-1;
    r->prod.head = r->cons.head = 0;
    r->prod.tail = r->cons.tail = 0;

//	struct element_info *element = NULL;
//	if ((element = (struct element_info*)malloc(sizeof(*element)))){
//		bzero(element, sizeof(*element));
//		element->address = (void*)r;
//		list_add_tail(&element->list, &process_info.queue_info_list);
//	}

	return r;
}

/*
 * change the high water mark. If *count* is 0, water marking is
 * disabled
 */
int rte_ring_set_water_mark(struct rte_ring *r, unsigned count)
{
    if (count >= r->prod.size)
        return -EINVAL;

    /* if count is 0, disable the watermarking */
    if (count == 0)
        count = r->prod.size;

    r->prod.watermark = count;
    return 0;
}

void rte_ring_free(struct rte_ring *r)
{
	free(r);
}
