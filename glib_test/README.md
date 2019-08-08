root@ubuntu:/workspace/glib_test# pkg-config --libs glib-2.0
-lglib-2.0
root@ubuntu:/workspace/glib_test# pkg-config --libs gobject-2.0
-lgobject-2.0 -lglib-2.0

g_object_new 属于gobject



/*
    生成thrift用的embezzleCreditClient结构体
    参   数:embezzle_credit_client_arg_t*
    返回值:dhcprpcembezzleCreditClient
*/
static dhcprpcEmbezzleCreditClient *report_embezzle_credit_client_stuff(credit_client_arg_t *credit_client_arg)
{
    char mac[20]                 = {0};
    char ipbuf[INET6_ADDRSTRLEN] = {0};

    uint8_t       *p        = NULL;
    GByteArray    *op82     = NULL;
    GByteArray    *op60     = NULL;
    dhcprpcIpAddr *relayIp  = NULL;
    GByteArray    *hostname = NULL;

    dhcprpcEmbezzleCreditClient *embezzleCreditClient = NULL;

    // create mac
    p = (uint8_t*)&credit_client_arg->mac;
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", p[0], p[1], p[2], p[3], p[4], p[5]);

    // create op82
    op82 = g_byte_array_new();
    g_byte_array_append(op82, credit_client_arg->op82, credit_client_arg->op82_len);

    // create op60
    op60 = g_byte_array_new();
    g_byte_array_append(op60, credit_client_arg->op60, credit_client_arg->op60_len);

    //create relayIp
    ip2string(&credit_client_arg->relayip, ipbuf, INET6_ADDRSTRLEN);

    relayIp = g_object_new(DHCPRPC_TYPE_IP_ADDR,
                           "version", IPV4,
                           "addr", ipbuf,
                           NULL);

    //create hostname
    hostname = g_byte_array_new();
    g_byte_array_append(hostname, credit_client_arg->hostname, credit_client_arg->hostname_len);

    embezzleCreditClient = g_object_new(DHCPRPC_TYPE_EMBEZZLE_CREDIT_CLIENT,
                                        "mac", mac,
                                        "op82", op82,
                                        "op60", op60,
                                        "relayIp", relayIp,
                                        "hostname", hostname,
                                        NULL);

    DHCP_DEBUG(LOG_LEVEL_DEBUG, "Add embezzleCreditClient report, mac:%s\trelayip:%s",
               mac,
               relayIp->addr);
    report_print_op("op60", op60);
    report_print_op("op82", op82);
    report_print_op("hostname", hostname);


    g_byte_array_unref(op82);
    g_byte_array_unref(op60);
    g_byte_array_unref(hostname);
    g_object_unref(relayIp);


    return embezzleCreditClient;
}



void print_ref(gpointer data)
{
    GByteArray    *op82     = NULL;
    GByteArray    *op60     = NULL;
    dhcprpcIpAddr *relayIp  = NULL;
    GByteArray    *hostname = NULL;
    /* g_object_get 也会增加引用计数 */
    g_object_get(data, "op60", &op60, "op82", &op82, "relayIp", &relayIp, "hostname", &hostname, NULL);
    g_byte_array_unref(op82);
    g_byte_array_unref(op60);
    g_byte_array_unref(hostname);

    print_byte_array_count("op82", op82);
    print_byte_array_count("op60", op60);
    print_byte_array_count("hostname", hostname);

}
