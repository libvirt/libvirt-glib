#include <config.h>
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <libvirt/libvirt.h>
#include <libvirt-glib/libvirt-glib.h>

#define DEBUG0(fmt) printf("%s:%d :: " fmt "\n", __FUNCTION__, __LINE__)
#define DEBUG(fmt, ...) printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__)
#define STREQ(a,b) (strcmp((a),(b)) == 0)

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

/* handle globals */
int h_fd = 0;
int h_event = 0;
virEventHandleCallback h_cb = NULL;
void *h_opaque = NULL;

/* timeout globals */
#define TIMEOUT_MS 1000
int t_timeout = -1;
virEventTimeoutCallback t_cb = NULL;
void *t_opaque = NULL;


/* Prototypes */
const char *eventToString(int event);
int myDomainEventCallback1 (virConnectPtr conn, virDomainPtr dom, int event, void *opaque);
int myDomainEventCallback2 (virConnectPtr conn, virDomainPtr dom, int event, void *opaque);

void usage(const char *pname);

/* Callback functions */

const char *eventToString(int event) {
    const char *ret = NULL;
    switch(event) {
        case VIR_DOMAIN_EVENT_ADDED:
            ret ="Added";
            break;
        case VIR_DOMAIN_EVENT_REMOVED:
            ret ="Removed";
            break;
        case VIR_DOMAIN_EVENT_STARTED:
            ret ="Started";
            break;
        case VIR_DOMAIN_EVENT_SUSPENDED:
            ret ="Suspended";
            break;
        case VIR_DOMAIN_EVENT_RESUMED:
            ret ="Resumed";
            break;
        case VIR_DOMAIN_EVENT_STOPPED:
            ret ="Stopped";
            break;
        case VIR_DOMAIN_EVENT_SAVED:
            ret ="Saved";
            break;
        case VIR_DOMAIN_EVENT_RESTORED:
            ret ="Restored";
            break;
        default:
            ret ="Unknown Event";
    }
    return ret;
}

int myDomainEventCallback1 (virConnectPtr conn ATTRIBUTE_UNUSED,
                            virDomainPtr dom,
                            int event,
                            void *opaque ATTRIBUTE_UNUSED)
{
    printf("%s EVENT: Domain %s(%d) %s\n", __FUNCTION__, virDomainGetName(dom), virDomainGetID(dom), eventToString(event));
    return 0;
}

int myDomainEventCallback2 (virConnectPtr conn ATTRIBUTE_UNUSED,
                            virDomainPtr dom,
                            int event,
                            void *opaque ATTRIBUTE_UNUSED)
{
    printf("%s EVENT: Domain %s(%d) %s\n", __FUNCTION__, virDomainGetName(dom), virDomainGetID(dom), eventToString(event));
    return 0;
}

/* main test functions */

void usage(const char *pname)
{
    printf("%s uri\n", pname);
}

int main(int argc, char **argv)
{
    GMainLoop *loop;

    if(argc > 1 && STREQ(argv[1],"--help")) {
        usage(argv[0]);
        return -1;
    }
    loop = g_main_loop_new(g_main_context_default(), 1);

    virEventRegisterGLib();

    virConnectPtr dconn = NULL;
    dconn = virConnectOpen (argv[1] ? argv[1] : "qemu:///system");
    if (!dconn) {
        printf("error opening\n");
        return -1;
    }

    DEBUG0("Registering domain event cbs");

    /* Add 2 callbacks to prove this works with more than just one */
    virConnectDomainEventRegister(dconn, myDomainEventCallback1, NULL);
    virConnectDomainEventRegister(dconn, myDomainEventCallback2, NULL);

    g_main_loop_run(loop);

    if (virConnectClose(dconn) < 0)
        printf("error closing\n");

    printf("done\n");
    return 0;
}

