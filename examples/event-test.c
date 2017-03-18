#include <config.h>
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <libvirt/libvirt.h>
#include <libvirt-glib/libvirt-glib.h>

#define STREQ(a,b) (strcmp((a),(b)) == 0)

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

/* Prototypes */
const char *eventToString(int event);
int myDomainEventCallback1 (virConnectPtr conn, virDomainPtr dom, int event, int detail, void *opaque);
int myDomainEventCallback2 (virConnectPtr conn, virDomainPtr dom, int event, int detail, void *opaque);

void usage(const char *pname);

/* Callback functions */

const char *eventToString(int event) {
    const char *ret = NULL;
    switch(event) {
        case VIR_DOMAIN_EVENT_DEFINED:
            ret ="Defined";
            break;
        case VIR_DOMAIN_EVENT_UNDEFINED:
            ret ="Undefined";
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
        default:
            ret ="Unknown Event";
    }
    return ret;
}

static const char *eventDetailToString(int event, int detail) {
    const char *ret = "";
    switch(event) {
    case VIR_DOMAIN_EVENT_DEFINED:
        if (detail == VIR_DOMAIN_EVENT_DEFINED_ADDED)
            ret = "Added";
        else if (detail == VIR_DOMAIN_EVENT_DEFINED_UPDATED)
            ret = "Updated";
        break;
    case VIR_DOMAIN_EVENT_UNDEFINED:
        if (detail == VIR_DOMAIN_EVENT_UNDEFINED_REMOVED)
            ret = "Removed";
        break;
    case VIR_DOMAIN_EVENT_STARTED:
        switch (detail) {
        case VIR_DOMAIN_EVENT_STARTED_BOOTED:
            ret = "Booted";
            break;
        case VIR_DOMAIN_EVENT_STARTED_MIGRATED:
            ret = "Migrated";
            break;
        case VIR_DOMAIN_EVENT_STARTED_RESTORED:
            ret = "Restored";
            break;
        default:
            break;
        }
        break;
    case VIR_DOMAIN_EVENT_SUSPENDED:
        if (detail == VIR_DOMAIN_EVENT_SUSPENDED_PAUSED)
            ret = "Paused";
        else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_MIGRATED)
            ret = "Migrated";
        break;
    case VIR_DOMAIN_EVENT_RESUMED:
        if (detail == VIR_DOMAIN_EVENT_RESUMED_UNPAUSED)
            ret = "Unpaused";
        else if (detail == VIR_DOMAIN_EVENT_RESUMED_MIGRATED)
            ret = "Migrated";
        break;
    case VIR_DOMAIN_EVENT_STOPPED:
        switch (detail) {
        case VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN:
            ret = "Shutdown";
            break;
        case VIR_DOMAIN_EVENT_STOPPED_DESTROYED:
            ret = "Destroyed";
            break;
        case VIR_DOMAIN_EVENT_STOPPED_CRASHED:
            ret = "Crashed";
            break;
        case VIR_DOMAIN_EVENT_STOPPED_MIGRATED:
            ret = "Migrated";
            break;
        case VIR_DOMAIN_EVENT_STOPPED_SAVED:
            ret = "Saved";
            break;
        case VIR_DOMAIN_EVENT_STOPPED_FAILED:
            ret = "Failed";
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return ret;
}


int myDomainEventCallback1 (virConnectPtr conn ATTRIBUTE_UNUSED,
                            virDomainPtr dom,
                            int event,
                            int detail,
                            void *opaque ATTRIBUTE_UNUSED)
{
    printf("%s EVENT: Domain %s(%d) %s: %s\n", __FUNCTION__,
           virDomainGetName(dom), virDomainGetID(dom),
           eventToString(event),
           eventDetailToString(event, detail));

    return 0;
}

int myDomainEventCallback2 (virConnectPtr conn ATTRIBUTE_UNUSED,
                            virDomainPtr dom,
                            int event,
                            int detail,
                            void *opaque ATTRIBUTE_UNUSED)
{
    printf("%s EVENT: Domain %s(%d) %s: %s\n", __FUNCTION__,
           virDomainGetName(dom), virDomainGetID(dom),
           eventToString(event),
           eventDetailToString(event, detail));

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

    gvir_init(&argc, &argv);

    if(argc > 1 && STREQ(argv[1],"--help")) {
        usage(argv[0]);
        return -1;
    }
    loop = g_main_loop_new(g_main_context_default(), 1);

    gvir_event_register();

    virConnectPtr dconn = NULL;
    dconn = virConnectOpen (argv[1] ? argv[1] : NULL);
    if (!dconn) {
        printf("error opening\n");
        return -1;
    }

    g_debug("Registering domain event cbs");

    /* Add 2 callbacks to prove this works with more than just one */
    virConnectDomainEventRegister(dconn, myDomainEventCallback1, NULL, NULL);
    virConnectDomainEventRegister(dconn, myDomainEventCallback2, NULL, NULL);

    g_main_loop_run(loop);

    if (virConnectClose(dconn) < 0)
        printf("error closing\n");

    printf("done\n");
    return 0;
}
