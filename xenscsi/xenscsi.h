#if !defined(_XENSCSI_H_)
#define _XENSCSI_H_

#include <ntifs.h>
#include <ntddk.h>
#include <wdm.h>
#include <initguid.h>
#include <ntdddisk.h>
//#include <srb.h>

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#define __DRIVER_NAME "XenSCSI"

#include <xen_windows.h>
#include <memory.h>
#include <grant_table.h>
#include <event_channel.h>
#include <hvm/params.h>
#include <hvm/hvm_op.h>
#include <xen_public.h>
#include <io/ring.h>
#include <io/vscsiif.h>

//#include <io/blkif.h>
#include <storport.h>
//#include <ntddscsi.h>
//#include <ntdddisk.h>
#include <stdlib.h>
#include <io/xenbus.h>
#include <io/protocols.h>


typedef struct vscsiif_request vscsiif_request_t;
typedef struct vscsiif_response vscsiif_response_t;

#define XENSCSI_POOL_TAG (ULONG) 'XSCS'

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define VSCSIIF_RING_SIZE __RING_SIZE((vscsiif_sring_t *)0, PAGE_SIZE)

typedef struct {
  vscsiif_request_t req;
  PSCSI_REQUEST_BLOCK Srb;
} vscsiif_shadow_t;

#define SHADOW_ENTRIES 32
#define MAX_GRANT_ENTRIES 512

#define SCSI_DEV_STATE_MISSING 0
#define SCSI_DEV_STATE_PRESENT 1
#define SCSI_DEV_STATE_ACTIVE  2

#define SCSI_DEV_NODEV ((ULONG)-1)

typedef struct {
  ULONG dev_no; // SCSI_DEV_NODEV == end
  USHORT channel;
  USHORT id;
  USHORT lun;
//  UCHAR state; /* SCSI_DEV_STATE_XXX */
} scsi_dev_t;

typedef struct {
  USHORT state;
  UCHAR devs[1024];
  UCHAR path[128];
  PUCHAR ptr;
} enum_vars_t;

struct
{
  vscsiif_shadow_t shadows[SHADOW_ENTRIES];
  USHORT shadow_free_list[SHADOW_ENTRIES];
  USHORT shadow_free;

  grant_ref_t grant_free_list[MAX_GRANT_ENTRIES];
  USHORT grant_free;
  USHORT grant_entries;

  evtchn_port_t event_channel;

  vscsiif_front_ring_t ring;

  int host;
  int channel;
  int id;
  int lun;

  XENPCI_VECTORS vectors;
} typedef XENSCSI_DEVICE_DATA, *PXENSCSI_DEVICE_DATA;

enum dma_data_direction {
        DMA_BIDIRECTIONAL = 0,
        DMA_TO_DEVICE = 1,
        DMA_FROM_DEVICE = 2,
        DMA_NONE = 3,
};

VOID
XenScsi_FillInitCallbacks(PHW_INITIALIZATION_DATA HwInitializationData);

#endif
