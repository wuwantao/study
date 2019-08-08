#include <stdio.h>
//#include <glib.h>
#include <glib-object.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
typedef struct _GRealArray  GRealArray;

/**
 * GArray:
 * @data: a pointer to the element data. The data may be moved as
 *     elements are added to the #GArray.
 * @len: the number of elements in the #GArray not including the
 *     possible terminating zero element.
 *
 * Contains the public fields of a GArray.
 */
struct _GRealArray
{
	guint8 *data;
	guint   len;
	guint   alloc;
	guint   elt_size;
	guint   zero_terminated : 1;
	guint   clear : 1;
	gint    ref_count;
	GDestroyNotify clear_func;
};
void test(gpointer tmp)
{
	GRealArray *rarray = NULL;
	rarray = (GRealArray*) tmp;
	printf("test ref:%d\n",rarray->ref_count);
}
int main()
{
	// create op82
	GByteArray *op82 = NULL;
	char op82_buff[1024] = {0};
	int i = 0;
	GRealArray *rarray = NULL;
	memset(op82_buff,1,1024);

	op82 = g_byte_array_new();
	rarray = (GRealArray*) op82;
	printf("ref:%d\n",rarray->ref_count);

	g_byte_array_append(op82, op82_buff, 1024);
	printf("ref:%d\n",rarray->ref_count);

	g_byte_array_unref(op82);
	printf("ref:%d\n",rarray->ref_count);

	return 0;
}
