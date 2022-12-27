#ifndef	_service
#define	_service

/* Module service */

#include <mach/kern_return.h>
#if	(defined(__STDC__) || defined(c_plusplus)) || defined(LINTLIBRARY)
#include <mach/port.h>
#include <mach/message.h>
#endif

#ifndef	mig_external
#define mig_external extern
#endif

mig_external void init_service
#if	(defined(__STDC__) || defined(c_plusplus))
    (port_t rep_port);
#else
    ();
#endif
#include <mach/std_types.h>

/* Routine old_service_checkin */
mig_external kern_return_t old_service_checkin
#if	defined(LINTLIBRARY)
    (service_request, service_desired, service_granted)
	port_t service_request;
	port_t service_desired;
	port_t *service_granted;
{ return old_service_checkin(service_request, service_desired, service_granted); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t service_request,
	port_t service_desired,
	port_t *service_granted
);
#else
    ();
#endif
#endif

/* Routine service_checkin */
mig_external kern_return_t service_checkin
#if	defined(LINTLIBRARY)
    (service_request, service_desired, service_granted)
	port_t service_request;
	port_t service_desired;
	port_t *service_granted;
{ return service_checkin(service_request, service_desired, service_granted); }
#else
#if	(defined(__STDC__) || defined(c_plusplus))
(
	port_t service_request,
	port_t service_desired,
	port_t *service_granted
);
#else
    ();
#endif
#endif

#endif	_service
