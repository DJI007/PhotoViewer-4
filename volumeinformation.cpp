#include "volumeinformation.h"

#include <QDebug>

extern "C" {
#include <stdio.h>
#include <mntent.h>
#include <blkid/blkid.h>
}

VolumeInformation::VolumeInformation()
{
    FILE* fstab;
    struct mntent *e;
    const char *devname = NULL;

    fstab = setmntent("/etc/mtab", "r");
    while ((e = getmntent(fstab))) {
        qDebug () << e->mnt_dir;

        if (strcmp("/", e->mnt_dir) == 0) {
            devname = e->mnt_fsname;
            break;
        }
    }

    qDebug () << "root devname is " << devname;

    blkid_probe pr;
    const char *uuid;

     //pr = blkid_new_probe_from_filename(devname);
     pr = blkid_new_probe_from_filename("/home/david/tmp/imagenes/0001_00001.MTS");
     if (!pr) {
        qDebug () << "Failed to open ";
     }
     else {
         blkid_do_probe(pr);
         blkid_probe_lookup_value(pr, "UUID", &uuid, NULL);

         qDebug () << "UUID=" <<  uuid;

         blkid_free_probe(pr);
     }

     endmntent(fstab);
}
