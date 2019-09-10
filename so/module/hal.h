/**
 * @mainpage QNAP HAL Application Programming Interface
 *
 * @section INTRO_SECTION Introduction
 * QNAP HAL Library is a hardware abstraction library which hides the
 * differences in real hardware layout, naming and scanning order from
 * various QNAP NAS hardware platform and provides uniform and platform
 * independent functions to retrieve the managed hardware configuration.
 *
 * @section Architecture
 * QNAP HAL is composed of the following components:
 * - HAL upper layer.
 * - HAL system mapping layer.
 * - HAL daemon.
 * - HAL mapping file.
 *
 * @section Prerequisite
 * - udev, sg_inq, sg_ses
 *
 * @section MODEL Supporting Model
 * - TS-X86
 *
 * @section COPYRIGHT Copyright
 * Copyright (c) 2011 QNAP SYSTEMS, INC.
 * All Rights Reserved.
 *
 */

/**
 * $Header: /home/cvsroot/NasX86/NasLib/hal/hal.h,v 1.286 2015/02/25 11:02:44 justinchen Exp $
 *
 * Copyright (c) 2011 QNAP SYSTEMS, INC.
 * All Rights Reserved.
 *
 * @brief       QNAP HAL function declaration file.
 * @author      Nike Chen
 * @date        2011/05/02
 *
 * $Id: hal.h,v 1.286 2015/02/25 11:02:44 justinchen Exp $
 */

#ifndef _QNAP_HAL_HDR
#define _QNAP_HAL_HDR

#ifdef __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <sys/stat.h>   // for dev_t declaration
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#define ENC_MAX_INTERNAL_ENCLOSURE_ID	31 /*!< from 1-31 */
#define ENC_MIN_EXTERNAL_ENCLOSURE_ID   (ENC_MAX_INTERNAL_ENCLOSURE_ID + 1)
#define ENC_CACHE_MIN_ENCLOSURE_ID		(ENC_MAX_INTERNAL_ENCLOSURE_ID - 8)		// Reserve 8 enc IDs for cache enclosure
#define ENC_ISCSI_ENCLOSURE_ID          ENC_MAX_INTERNAL_ENCLOSURE_ID
#define ENC_MAX_STACK_NUM		4
#define ENC_MAX_THUNDERBOLT_STACK_NUM   6

#define DEFAULT_WWN_LEN                 8
#define NULL_WWN_STR                    "0000000000000000"
#define MAX_SAS_HBA_SN                  20
#define MAX_SAS_HBA_NUM                 10
#define MAX_PHY_NUM_PER_PORT            8
#define MAX_PHY_NUM_OF_HBA              16
#define MAX_PHY_NUM_OF_REXP             64          /*!< Should include virtual phy */
#define MAX_PORT_NUM_OF_REXP            3
#define MAX_PORT_NUM_OF_HBA             16
#define MAX_DATE_LEN                    16
#define MAX_PD_DEV_NAME                 32
#define MAX_SYS_ID_LEN                  32
#define MAX_DRIVER_NAME                 32
#define MAX_WWN_LEN                     MAX_SERIAL_NO_LEN        /* 128-bits + '\0' */
#define MAX_UID_LEN                     MAX_WWN_LEN
#define MAX_VENDOR_LEN                  32
#define MAX_MODEL_LEN                   (40 + 1)        /* 40 bytes + '\0', limited by ATA identify cmd */
#define MAX_NIC_MODEL_LEN               256
#define MAX_SERIAL_NO_LEN               (32 + 1)
#define MAX_REVISION_LEN                16
#define MAX_PD_SYS_NAME                 32
#define MAX_PD_SLOT_NAME                32
#define MAX_PD_CTRL_NAME                32
#define MAX_PRT_CTRL_NAME               32
#define MAX_PD_BUS_NAME                 32
#define MAX_CONF_FILE_PATH              64
#define MAX_ID_STR_LEN                  (32 + 1)
#define MAX_SECTION_LEN                 (32 + MAX_SYS_ID_LEN)
#define DEFAULT_FAN_STEPPING            20
#define MAX_RAW_DATA_LEN 4096   // scsi log page used, sg util used 0xffff
#define MAX_EXTRA_FAN_REGION_COUNT      2
#define MAX_EXTRA_TEMP_REGION_COUNT     2
#define MAX_FAN_REGION_ARRAY            8
#define MAX_TEMP_REGION_ARRAY           8

#define MAX_SE_NUM                      24 /* maximum enclsoure number (including NAS) */
#define MAX_PD_NUM                      64 /* maximum physical disk number in each enclosure */
#define MAX_DRIVES_SUPPORT		       (MAX_SE_NUM * MAX_PD_NUM)
#define MAX_PRT_NUM                     16 /* maximum printer number */
#define MAX_NAME_SIZE                   128
#define MAX_IDX_SIZE                    32
#define MAX_LOG_LEN			256
#define MAX_EXT_CONNECTOR_NUM		8
#define MAX_ERASE_COUNT                 5  /* Maximun erase count */
#define MAX_MAC_BIN_LEN                 6  /* 48-bits MAC address */
#define MAX_MAC_ASCII_LEN               18 /* in the form of "00:11:22:33:44:55" */

#define MAX_PD_LIST_LEN                 (MAX_PD_NUM * 10)

#define HAL_CONF_FILE                   "/etc/hal.conf"
#define HAL_CONF_BDISK_SEC              "BootDisk_%d"
#define HAL_CONF_GLOBAL_SEC             "Global"
#define HAL_CONF_INDEX_SEC              "Index"
#define HAL_CONF_ENC_SEC                "Enclosure_%d"

#define PD_ATTACH_CALLBACK              "/etc/init.d/pd_attach"
#define PD_DETACH_CALLBACK              "/etc/init.d/pd_detach"
#define PD_SMART_TMP_DIR                "/tmp/smart"
#define PD_SMART_TMP_LOCK_FILE          "/var/lock/smart_tmp.lck"
#define PD_SMART_WARNING_TMP_CONF_FILE   "/tmp/smart_warning.conf"        /*!< S.M.A.R.T. Warning config file path under /tmp. */
#define PD_SMART_WARNING_CONF_FILE       "/etc/config/smart_warning.conf" /*!< S.M.A.R.T. Warning config file path. */
#define PD_SMART_WARNING_CONF_SECTION    "S.M.A.R.T. ID #%d"              /*!< S.M.A.R.T. Warning config section. */
#define PD_SMART_WARNING_CONF_POLICY_KEY "policy"                         /*!< S.M.A.R.T. Warning config policy of key. */
#define PD_SMART_WARNING_CONF_ENABLE_KEY "enable"                         /*!< S.M.A.R.T. Warning config enable of key. */
#define PD_SMART_WARNING_CONF_VALUE_KEY  "value"                          /*!< S.M.A.R.T. Warning config value of key. */
#define MAX_SMART_ATTR_NAME_LEN         64

// SED Jimmy 20181123 
#define CMD_SEDUTIL_MAX_LEN				128
#define CMD_SEDUTIL      				"/sbin/sedutil-cli"
#define SED_LOCK_FILE                	"/tmp/.sed_lock_file"
#define SED_INFO_CONF                	"/tmp/.sedinfo.conf"



#define SED_OPTION_ON					"on"
#define SED_OPTION_OFF					"off"
#define SED_OPTION_LOCK					"LK"
#define SED_OPTION_RW					"RW"

#define SED_ACTION_SCAN					"--scan"
#define SED_ACTION_IS_SED				"--isValidSED"
#define SED_ACTION_QUERY				"--query"
#define SED_ACTION_INIT_SETUP			"--initialSetup"
#define SED_ACTION_SET_MBRE				"--setMBREnable"
#define SED_ACTION_SETUP_LOCK_RANGE		"--setupLockingRange"
#define SED_ACTION_LIST_LOCK_RANGE		"--listLockingRanges"
#define SED_ACTION_ENABLE_LOCK_RANGE	"--enableLockingRange"
#define SED_ACTION_DISABLE_LOCK_RANGE	"--disableLockingRange"
#define SED_ACTION_SET_LOCK_RANGE		"--setLockingRange"
#define SED_ACTION_SET_ADMIN_PASS		"--setAdmin1Pwd"
#define SED_ACTION_SET_SID_PASS			"--setSIDPassword"
#define SED_ACTION_REVERT_NO_ERASE		"--revertNoErase"
#define SED_ACTION_REVERT_TPER			"--revertTPer"
#define SED_ACTION_ERASE_BY_PSID		"--yesIreallywanttoERASEALLmydatausingthePSID"

#define SED_REDIR						"2>/dev/null"//"2>/dev/null &"
#define SED_REDIR1						"&>/dev/null 2>>/dev/null"



// SAS Physical Link status
#define PHY_LINK_LOSS			0x01
#define PHY_LINK_MISCONNECT		0x02
#define PHY_LINK_IN_PORT		0x04

//[SDMD START] 20130121 by csw: for SDM used
#define SDM_ENABLE                      "Enabled"
#define NAS_CONF_SDM_SEC                "SDM"
//[SDMD END]

// For disk data collection
#define NAS_DISK_DATA_COLLECTION_SCRIPT "/etc/init.d/disk_data_collection.sh"
#define NAS_DISK_DATA_IOTEST_SCRIPT 	"/etc/init.d/da_iotest.sh"
#define NAS_CONF_DISK_DATA_SECTION      "Disk Data Collection"
#define NAS_CONF_DISK_DATA_ENABLED      "Enabled"
#define NAS_DISK_DATA_LOCATION	        "/share/CACHEDEV1_DATA/.system_disk_data"

#define NAS_CONF_FILE                   "/etc/config/uLinux.conf"

#define QNAP_SR_DEVICE_A2AR             "A2AR"
#define QNAP_SR_DEVICE_A2MAR            "A2MAR"
#define HAL_SR_RAID_CONF_FILE           "/etc/config/sr_raid.conf"
#define HAL_TR_RAID_CONF_FILE           "/etc/config/tr_raid.conf"
#define HAL_TR_CONF_GLOBAL_SEC          "Global"
#define HAL_TR_CONF_RAID_SEC            "RAID_%d"
#define HAL_TR_CONF_RAID_BITMAP_FIELD   "raidBitmap"
#define HAL_TR_CONF_DATA_BITMAP_FIELD   "dataBitmap"
#define HAL_TR_CONF_DATA_FIELD          "data_%d"
#define HAL_TR_CONF_ID_FIELD            "id"
#define HAL_TR_CONF_UUID_FIELD          "uuid"
#define HAL_TR_CONF_RAIDLEVEL_FIELD     "raidLevel"
#define HAL_TR_CONF_FW_UPGRADE          "/etc/tr_fw_upgrade.conf"
#define HAL_TR_CONF_FW_UPGRADE_FIELD    "is_upgrading"
#define HAL_TR_VERSION_SCRIPT           "/etc/init.d/gen_tr_version_conf.sh"
#define HAL_TR_VERSION_XML_DEFAULT      "/tmp/RAIDManager.xml"
#define HAL_TR_VERSION_XML              "/etc/RAIDManager.xml"
#define HAL_TR_VERSION_CONF_DEFAULT     "/etc/tr_version.conf"
#define HAL_TR_VERSION_CONF_LATEST      "/etc/latest_tr_version.conf"
#define HAL_TR_VERSION_CONF             "/etc/config/tr_version.conf"
#define HAL_TR_VERSION_SECTION          "firmware_%d"
#define HAL_TR_MODEL_SECTION            "model"
#define HAL_TR_VERSION_JMB393_FIELD     "JMB393_version"
#define HAL_TR_VERSION_JMS576_FIELD     "JMS576_version"
#define HAL_TR_VERSION_JMS562_FIELD     "JMS562_version"
#define HAL_TR_VERSION_JMS580_FIELD     "JMS580_version"
#define HAL_TR_VERSION_FIELD            "version"
#define HAL_TR_LOCK_FILE                "/tmp/.tr_lock_file"
#define HAL_TR_VERSION_CONF_LOCK_FILE   "/tmp/.tr_version_conf_lock_file"
#define HAL_SR_LOCK_FILE                "/tmp/.sr_lock_file"
#define HAL_TR_CONF_ERR_STATUS_FIELD    "ERR_STATUS"
#define HAL_TR_CONF_CONFIG_RAID_FIELD   "CONFIG_RAID"
#define HAL_SR_INFO_CACHE_FILE            "/tmp/.sr_cache_info"
#define HAL_SR_INFO_RAID_LEVEL            "raid_level"
#define HAL_SR_INFO_RAID_DEV_NUM          "dev_num"
#define HAL_SR_INFO_RAID_STATUS           "raid_status"
#define HAL_SR_INFO_RAID_REBUILD_PRIORITY "rebuild_priority"
#define HAL_SR_INFO_RAID_ARRAY_SIZE       "array_size"
#define HAL_SR_INFO_RAID_REBUILD_PERCENT  "rebuild_percent"

#define HAL_TLS_LOCK_FILE                "/tmp/.tls_lock_file"

#define EM_INFO_DIR                     "/tmp/em"
#define MAX_EM_INFO_PWR_COUNT           2
#define MAX_EM_INFO_FAN_COUNT           6
#define MAX_EM_INFO_CPU_COUNT           4
#define MAX_EM_INFO_ENV_COUNT           32
#define ALL_ENCLOSURES                  ((int) -1)
#define ALL_DEVICE                      ((int) -1)
#define MAX_BOOT_PD_NUM			2

#define SE_MTD_BLK_DEVICE               "/dev/mtdblock"
#define SE_EMMC_BLK_DEVICE              "/dev/mmcblk0p"

#define SE_HOTSWAP_FILE                  "/tmp/.se_detached"
#define SE_HOTSWAP_ENC_SEC               "Enclosure_%d"
#define SE_HOTSWAP_DETACHED_FIELD        "detached"

#define TR_CONFIG_RAID_RECORD_FILE      "/tmp/tr_config_raid_record.info"
//  SWChen 20131002 fix bug 37938
#define CMD_RMMOD_EHCI_HCD              "/sbin/rmmod ehci-hcd.ko"
#define CMD_INSMOD_EHCI_HCD             "/sbin/insmod /lib/modules/misc/ehci-hcd.ko"
#define REDIR_TO_NULL                   " &>/dev/null"

// bug 48045 NCQ enable/disable scheme
#define QNAP_NCQ_BUF                    8
#define MAX_QNAP_NCQ_PATH               64
#define PD_QNAP_NCQ_PATH                "/sys/block/%s/device/qnap_ncq"
#define MAX_NCQ_BLACKLIST_PATH          64
#define MAX_SMART_SELFTEST_BLACKLIST_PATH          64
#define NCQ_BLACKLIST_PATH              "/etc/ncq_blacklist/%s"
#define SMART_SELFTEST_BLACKLIST_PATH              "/etc/smart_selftest_blacklist/%s"
#define WWN_BLACKLIST_PATH              "/etc/wwn_blacklist/%s"
// delimiters is "##", so we add 4 char for safety
#define MAX_NCQ_BLACKLIST_LINE          MAX_MODEL_LEN + MAX_REVISION_LEN + 4
#define NCQ_DEFAULT_FILE                "/etc/ncq_blacklist/default"
#define NCQ_DEFAULT_SECTIOON            "Global"
#define NCQ_DEFAULT_FIELD               "default"
#define NCQ_CONF_FILE                   "/etc/config/ncq_setting"
#define NCQ_CONF_DISK_SECTIOON          "Disk"
#define NCQ_SKIP_BLACKLIST              "Skip_blacklist"

#define SMART_SELFTEST_DEFAULT_FILE                "/etc/ncq_blacklist/default"
#define SMART_SELFTEST_DEFAULT_SECTIOON            "Global"
#define SMART_SELFTEST_SKIP_BLACKLIST              "Skip_blacklist"

/* Block Device Queue Depth reconfiguration */
#define BDQD_BLACKLIST_PATH             "/etc/bdqd_blacklist"

//Get Profile Temperature
#define HAL_UTIL_CONF "/etc/hal_util.conf"
#define HAL_UTIL_EXT_CONF "/etc/hal_util_ext.conf"
#define HAL_MQTS_TEMP_CONF "/etc/mqts_temperature.conf"
#define SHUTDOWN_TEMP_SECTION "SHUTDOWN TEMP"
#define SHUTDOWN_TEMP_FIELD "%s_TEMP"
#define CPU_THROTTLING_SECTION "CPU THROTTLING"
#define HAL_UTIL_CONF_SYSTEM_SECTION  "System Enclosure"
#define SMART_FAN_METHOD    "SMART_FAN_METHOD"
#define SMART_FAN_TFAN      "TFAN"
#define HAL_UTIL_CONF_TFAN_SYS_SECTION      "TFAN SYS"
#define HAL_UTIL_CONF_TFAN_CPU_SECTION      "TFAN CPU"
#define HAL_UTIL_CONF_TFAN_REG_SECTION      "TFAN REG %d"
#define HAL_UTIL_CONF_USING_SCROLLBAR_SECTION      "USING SCROLLBAR"
#define HAL_UTIL_CONF_TFAN_OPTIONAL_MODES      "OPTIONAL MODES"

// JeremySu 2016/11/21 Spec#7246
#define MAX_PD_TYPE_NAME        10
#define PD_TYPE_SSD             "SSD"
#define PD_TYPE_HDD             "HDD"

// SWChen 20131122 Redmine 6529 enhance event log
#define HOST_LOG_NAME "Host"
#define JBOD_LOG_NAME "REXP"
#define ROOT_USB_LOG_NAME "USB"
#define QM2_LOG_NAME "QM2"
#ifdef VIRTUAL_JBOD
#define VJBOD_LOG_NAME "Virtual JBOD"
#endif

// Boot recovery support
#define PLATFORM_CONF_FILE "/etc/platform.conf"
#define PLATFORM_CONF_FW_SECTION "FIRMWARE STORAGE"
#define PLATFORM_CONF_FW_FIELD_DEVICE_NODE "DEVICE_NODE"
#define PLATFORM_CONF_FW_FIELD_UPDATE_TMP_PARTITION "UPDATE_TMP_PARTITION"
#define PLATFORM_CONF_FW_FIELD_FS_ACTIVE_PARTITION "FS_ACTIVE_PARTITION"
#define PLATFORM_CONF_FW_FIELD_FS_TYPE "FS_TYPE"
#define PLATFORM_CONF_FW_FIELD_FS_RECOVERY_PID "FS_RECOVERY_PID"
#define PLATFORM_CONF_CONFIG_SECTION "CONFIG STORAGE"
#define PLATFORM_CONF_CONFIG_FIELD_DEVICE_NODE "DEVICE_NODE"
#define PLATFORM_CONF_CONFIG_FIELD_FS_TYPE "FS_TYPE"
#define PLATFORM_CONF_CONFIG_FIELD_FS_ACTIVE_PARTITION "FS_ACTIVE_PARTITION"
#define PLATFORM_CONF_MISC_SECTION "MISC"
#define PLATFORM_CONF_MISC_FIELD_DISPLAY_NAME "DISPLAY_NAME"
#define PLATFORM_CONF_FIELD_PLATFORM "Platform"

// Boot change model name for NVR. add by roylin 20141023
#define NVR_MODEL_DEF_FILE "/etc/default_config/uLinux.conf"
#define NVR_MODEL_XML_FILE "/usr/nvr/model.xml"

// BurtonLiang 20140814
#define LSI_BUILDIN_SAS2_HBA		2
#define LSI_BUILDIN_SAS3_HBA		3

#define LSI_SAS2_FLASH_UTILITY		"sas2flash"
#define LSI_SAS3_FLASH_UTILITY		"sas3flash"

// Memory modules
#define MEM_CGROUP_PATH "/sys/fs/cgroup/memory"
#define CGROUP_EVENT_CTL       "cgroup.event_control"
#define MEM_CGROUP_QNAP_RSS        "memory.qnap_rss_in_bytes"

#ifdef QTS_ZFS
#define ZFS_DISK_SYMLINK_PATH           "/dev/qzfs/enc_%d"
#define ZFS_DISK_SYMLINK_FORMAT         "disk_%s_%d"
#define ZFS_DISK_SYMLINK_MAX_LEN        (MAX_WWN_LEN + 32)
#endif

#define TR_MAX_UUID_LEN		    128

// Debug Macro
#define KMSG(x, ...) ({FILE *k;if((k=fopen("/dev/kmsg","r+"))!=NULL){fprintf(k,"---- "x"\n",##__VA_ARGS__);fclose(k);}})
#define KDBG(x,y, ...) ({struct stat s;FILE *k;if(!stat(x,&s)){if((k=fopen("/dev/kmsg","r+"))!=NULL){fprintf(k,"---- [%s@%s:%d] "y"\n",__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__);fclose(k);}}})
#define DBG_ON(x) ({struct stat s;stat(x,&s)?0:1;})

// Debug Control File List
#define DBG_ALL                             "/"
#define DBG_SKIP_PD_HOT_ADD_REMOVE          "/tmp/dbg_skip_pd_hot_add_remove"

// vQTS Cloud Global Definition
#define VQTS_CLOUD_PD_MIN_SIZE              20480 //20480 MB

enum {
    /* bits in ATA command block registers */
    ATA_DRDY        = (1 << 6), /* device ready */
    ATA_ICRC        = (1 << 7), /* interface CRC error */
    ATA_UNC         = (1 << 6), /* uncorrectable media error */
    ATA_IDNF        = (1 << 4), /* ID not found */
    ATA_ABORTED     = (1 << 2), /* command aborted */
    ATA_AMNF        = (1 << 0), /* address mark not found */

    /* ATA device commands */
    ATA_CMD_DEV_RESET	= 0x08, /* ATAPI device reset */
    ATA_CMD_CHK_POWER	= 0xE5, /* check power mode */
    ATA_CMD_STANDBY		= 0xE2, /* place in standby power mode */
    ATA_CMD_IDLE		= 0xE3, /* place in idle power mode */
    ATA_CMD_EDD		= 0x90,	/* execute device diagnostic */
    ATA_CMD_DOWNLOAD_MICRO  = 0x92,
    ATA_CMD_DOWNLOAD_MICRO_DMA = 0x93,
    ATA_CMD_NOP		= 0x00,
    ATA_CMD_FLUSH		= 0xE7,
    ATA_CMD_FLUSH_EXT	= 0xEA,
    ATA_CMD_ID_ATA		= 0xEC,
    ATA_CMD_ID_ATAPI	= 0xA1,
    ATA_CMD_SERVICE		= 0xA2,
    ATA_CMD_READ		= 0xC8,
    ATA_CMD_READ_EXT	= 0x25,
    ATA_CMD_READ_QUEUED	= 0x26,
    ATA_CMD_READ_STREAM_EXT	= 0x2B,
    ATA_CMD_READ_STREAM_DMA_EXT = 0x2A,
    ATA_CMD_WRITE		= 0xCA,
    ATA_CMD_WRITE_EXT	= 0x35,
    ATA_CMD_WRITE_QUEUED	= 0x36,
    ATA_CMD_WRITE_STREAM_EXT = 0x3B,
    ATA_CMD_WRITE_STREAM_DMA_EXT = 0x3A,
    ATA_CMD_WRITE_FUA_EXT	= 0x3D,
    ATA_CMD_WRITE_QUEUED_FUA_EXT = 0x3E,
    ATA_CMD_FPDMA_READ	= 0x60,
    ATA_CMD_FPDMA_WRITE	= 0x61,
    ATA_CMD_NCQ_NON_DATA	= 0x63,
    ATA_CMD_FPDMA_SEND	= 0x64,
    ATA_CMD_FPDMA_RECV	= 0x65,
    ATA_CMD_PIO_READ	= 0x20,
    ATA_CMD_PIO_READ_EXT	= 0x24,
    ATA_CMD_PIO_WRITE	= 0x30,
    ATA_CMD_PIO_WRITE_EXT	= 0x34,
    ATA_CMD_READ_MULTI	= 0xC4,
    ATA_CMD_READ_MULTI_EXT	= 0x29,
    ATA_CMD_WRITE_MULTI	= 0xC5,
    ATA_CMD_WRITE_MULTI_EXT	= 0x39,
    ATA_CMD_WRITE_MULTI_FUA_EXT = 0xCE,
    ATA_CMD_SET_FEATURES	= 0xEF,
    ATA_CMD_SET_MULTI	= 0xC6,
    ATA_CMD_PACKET		= 0xA0,
    ATA_CMD_VERIFY		= 0x40,
    ATA_CMD_VERIFY_EXT	= 0x42,
    ATA_CMD_WRITE_UNCORR_EXT = 0x45,
    ATA_CMD_STANDBYNOW1	= 0xE0,
    ATA_CMD_IDLEIMMEDIATE	= 0xE1,
    ATA_CMD_SLEEP		= 0xE6,
    ATA_CMD_INIT_DEV_PARAMS	= 0x91,
    ATA_CMD_READ_NATIVE_MAX	= 0xF8,
    ATA_CMD_READ_NATIVE_MAX_EXT = 0x27,
    ATA_CMD_SET_MAX		= 0xF9,
    ATA_CMD_SET_MAX_EXT	= 0x37,
    ATA_CMD_READ_LOG_EXT	= 0x2F,
    ATA_CMD_WRITE_LOG_EXT	= 0x3F,
    ATA_CMD_READ_LOG_DMA_EXT = 0x47,
    ATA_CMD_WRITE_LOG_DMA_EXT = 0x57,
    ATA_CMD_TRUSTED_NONDATA	= 0x5B,
    ATA_CMD_TRUSTED_RCV	= 0x5C,
    ATA_CMD_TRUSTED_RCV_DMA = 0x5D,
    ATA_CMD_TRUSTED_SND	= 0x5E,
    ATA_CMD_TRUSTED_SND_DMA = 0x5F,
    ATA_CMD_PMP_READ	= 0xE4,
    ATA_CMD_PMP_READ_DMA	= 0xE9,
    ATA_CMD_PMP_WRITE	= 0xE8,
    ATA_CMD_PMP_WRITE_DMA	= 0xEB,
    ATA_CMD_CONF_OVERLAY	= 0xB1,
    ATA_CMD_SEC_SET_PASS	= 0xF1,
    ATA_CMD_SEC_UNLOCK	= 0xF2,
    ATA_CMD_SEC_ERASE_PREP	= 0xF3,
    ATA_CMD_SEC_ERASE_UNIT	= 0xF4,
    ATA_CMD_SEC_FREEZE_LOCK	= 0xF5,
    ATA_CMD_SEC_DISABLE_PASS = 0xF6,
    ATA_CMD_CONFIG_STREAM	= 0x51,
    ATA_CMD_SMART		= 0xB0,
    ATA_CMD_MEDIA_LOCK	= 0xDE,
    ATA_CMD_MEDIA_UNLOCK	= 0xDF,
    ATA_CMD_DSM		= 0x06,
    ATA_CMD_CHK_MED_CRD_TYP = 0xD1,
    ATA_CMD_CFA_REQ_EXT_ERR = 0x03,
    ATA_CMD_CFA_WRITE_NE	= 0x38,
    ATA_CMD_CFA_TRANS_SECT	= 0x87,
    ATA_CMD_CFA_ERASE	= 0xC0,
    ATA_CMD_CFA_WRITE_MULT_NE = 0xCD,
    ATA_CMD_REQ_SENSE_DATA  = 0x0B,
    ATA_CMD_SANITIZE_DEVICE = 0xB4,
    ATA_CMD_ZAC_MGMT_IN	= 0x4A,
    ATA_CMD_ZAC_MGMT_OUT	= 0x9F,

    /* READ/WRITE LONG (obsolete) */
    ATA_CMD_READ_LONG	= 0x22,
    ATA_CMD_READ_LONG_ONCE	= 0x23,
    ATA_CMD_WRITE_LONG	= 0x32,
    ATA_CMD_WRITE_LONG_ONCE	= 0x33,

    /* SError bits */
    SERR_DATA_RECOVERED = (1 << 0), /* recovered data error */
    SERR_COMM_RECOVERED = (1 << 1), /* recovered comm failure */
    SERR_DATA       = (1 << 8), /* unrecovered data error */
    SERR_PERSISTENT     = (1 << 9), /* persistent data/comm error */
    SERR_PROTOCOL       = (1 << 10), /* protocol violation */
    SERR_INTERNAL       = (1 << 11), /* host internal error */
    SERR_PHYRDY_CHG     = (1 << 16), /* PHY RDY changed */
    SERR_PHY_INT_ERR    = (1 << 17), /* PHY internal error */
    SERR_COMM_WAKE      = (1 << 18), /* Comm wake */
    SERR_10B_8B_ERR     = (1 << 19), /* 10b to 8b decode error */
    SERR_DISPARITY      = (1 << 20), /* Disparity */
    SERR_CRC        = (1 << 21), /* CRC error */
    SERR_HANDSHAKE      = (1 << 22), /* Handshake error */
    SERR_LINK_SEQ_ERR   = (1 << 23), /* Link sequence error */
    SERR_TRANS_ST_ERROR = (1 << 24), /* Transport state trans. error */
    SERR_UNRECOG_FIS    = (1 << 25), /* Unrecognized FIS */
    SERR_DEV_XCHG       = (1 << 26), /* device exchanged */
};
/**
 * @defgroup HAL_UPPER_LAYER HAL Upper Layer
 */
/*@{*/  /* begin of upper layer module */

/**
 * @defgroup LIB_UTILITY Misc. Utility function
 */
/*@{*/  /* begin of utility module */

/**
 * @enum        SIZE_UNIT
 * @brief       Size conversion unit.
 */
typedef enum _SIZE_UNIT
{
    SIZE_DEFAULT = 0,   /*!< Shortest format */
    SIZE_BYTE,          /*!< Byte unit */
    SIZE_KB,            /*!< Kilobyte unit */
    SIZE_MB,            /*!< Megabyte unit */
    SIZE_GB,            /*!< Gigabyte unit */
    SIZE_TB,            /*!< Terabyte unit */
    SIZE_PB,            /*!< Petabyte unit */
    SIZE_EB,            /*!< Eurabyte unit */
    SIZE_LAST,          /*!< Invalid unit */
} SIZE_UNIT;

/**
 * @enum      PHY_LINK_SPEED
 * @brief     SAS/SATA link speed.
 */
typedef enum _PHY_LINK_SPEED
{
    PHY_LINK_NONE = -1,             /*!< No link connect. */
    PHY_LINK_1_5G = 1,              /*!< 1.5G link connect. */
    PHY_LINK_3_G = 3,               /*!< 3G link connect. */
    PHY_LINK_6_G = 6,               /*!< 6G link connect. */
} PHY_LINK_SPEED;

/**
 * @enum      VPD_LOC
 * @brief     VPD location
 */
typedef enum _VPD_LOC
{
    VPD_MB = 0,                  /*!< VPD on Main board */
    VPD_BP = 1,                  /*!< VPD on Back plane */
    VPD_BP2 = 2,                  /*!< VPD on the second Back plane */
    VPD_BP3 = 3,                  /*!< VPD on the second Back plane */
    VPD_LAST,
} VPD_LOC;

/**
 * @enum      C2F_RSVD_SIZE
 * @brief     C2F reserved mameory size
 */
typedef enum _C2F_RSVD_SIZE
{
    C2F_RSVD_INVALID = -1,       /*!< Keep setting for C2F*/
    C2F_RSVD_1G = 1,             /*!< Reserved 1G for C2F*/
    C2F_RSVD_2G = 2,             /*!< Reserved 2G for C2F*/
    C2F_RSVD_4G = 4,             /*!< Reserved 4G for C2F*/
    C2F_RSVD_8G = 8,             /*!< Reserved 8G for C2F*/
} C2F_RSVD_SIZE;

/**
 * @enum      LED_BV_CTRL
 * @brief     led brightness value control
 */
typedef enum _LED_BV_CTRL
{
    LED_BV_INVALID = -1,              /*!< Can not control led brightness value */
    LED_BV_GPIO = 0,                  /*!< Use GPIO to control  led brightness value*/
    LED_BV_PWM = 1,                   /*!< Use PWM to control  led brightness value*/
} LED_BV_CTRL;

/**
 * @fn char* Get_Size_Str(IN unsigned long long capacity, IN unsigned int block_size,
 *                      IN SIZE_UNIT unit, IN OUT char* size_str, IN unsigned int buf_len)
 * @brief Convert the specified capacity according to the specified size unit.
 *
 * This function will caculate the specified capacity based on the specified size unit.
 * @param[in] capacity Storage capacity in terms of block.
 * @param[in] block_size Storage block size in terms of byte.
 * @param[in] unit The conversion unit.
 * @param[out] size_str The converted output string will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval size_str Return the converted output string.
 */
char* Get_Size_Str(IN unsigned long long capacity, IN unsigned int block_size,
                   IN SIZE_UNIT unit, IN OUT char* size_str, IN unsigned int buf_len);

/**
 * @fn int Translate_MAC_ASCII_To_Bin(IN char* mac_ascii, OUT unsigned char mac_bin[], IN unsigned int buf_len)
 * @brief Translate the MAC ASCII string to binary blob.
 *
 * This function will translate the MAC ASCII string to the corresponding binary code.
 * @param[in] mac_ascii MAC in the ascii string format. (00:11:22:33:44:55 or 001122334455)
 * @param[out] mac_bin The converted binary code will be stored here.
 * @param[in] buf_len The length of output binary buffer.
 * @retval size_str Return the converted output binary buffer length.
 */
int Translate_MAC_ASCII_To_Bin(IN char* mac_ascii, OUT unsigned char mac_bin[], IN unsigned int buf_len);

/**
 * @fn int Translate_MAC_Bin_To_ASCII(IN unsigned char mac_bin[], IN unsigned int bin_buf_len,
 *                                    OUT char mac_ascii[], IN unsigned int ascii_buf_len)
 *
 * @brief Translate the MAC binary code to the corresponding ASCII string.
 *
 * This function will translate the MAC binary code to ASCII string.
 * @param[in] mac_bin The MAC binary code.
 * @param[in] bin_buf_len The length of the input binary code.
 * @param[out] mac_ascii MAC in the ascii string format. (00:11:22:33:44:55)
 * @param[in] ascii_buf_len The converted ascii string will be stored here.
 * @retval >= 0 Return the converted output string length.
 */
int Translate_MAC_Bin_To_ASCII(IN unsigned char mac_bin[], IN unsigned int bin_buf_len,
                               OUT char mac_ascii[], IN unsigned int ascii_buf_len);

/**
 * @fn int HAL_Is_S3_Enable(void)
 * @brief Check if the system could enter to S3 mode.
 *
 * @retval 1 Yes.
 * @retval 0 No.
 */
int HAL_Is_S3_Enable(void);

/*@}*/  /* end of utility module */

/**
 * @defgroup HAL_PHYSICAL_DISK Physical Disk function
 */

/**
 * @ingroup HAL_PHYSICAL_DISK Physical Disk function
 */
/*@{*/  /* begin of physical disk module */

/**
 * @enum        PD_LINK_SPEED
 * @brief       Physical Disk Link Speed.
 */
typedef enum _PD_LINK_SPEED
{
    PD_UNKNOWN_SPEED = 0,               /*!< Unknown link speed. */
    PD_USB_LS,                          /*!< USB low speed */
    PD_USB_FS,                          /*!< USB full speed */
    PD_USB_HS,                          /*!< USB high speed */
    PD_USB_SS,                          /*!< USB super speed (5Gbps USB3.0)*/
    PD_USB_SS_3_1_Gen1,                 /*!< USB super speed (5Gbps USB3.1 Gen1)*/
    PD_USB_SSP,                         /*!< USB super speed Plus (10Gbps USB3.1)*/
    PD_SATA_SAS_1_5G,                   /*!< Link 1.5 Gbps */
    PD_SATA_SAS_3G,                     /*!< Link 3 Gbps */
    PD_SATA_SAS_6G,                     /*!< Link 6 Gbps */
    PD_SATA_SAS_12G,                    /*!< Link 12 Gbps */
    PD_PCIE_2_5G=100,                   /*!< 2.5G link connect. */
    PD_PCIE_5G=200,                     /*!< 5G link connect. */
    PD_PCIE_8G=300,                     /*!< 8G link connect. */
    PD_PCIE_16G=400,                    /*!< 16G link connect. */
} PD_LINK_SPEED;

typedef struct _SATA_PHY_ERR_CNT_INFO
{
    uint8_t valid;
    uint64_t cnt;
} SATA_PHY_ERR_CNT_INFO;
typedef struct _SATA_PHY_EVENT_INFO
{
    SATA_PHY_ERR_CNT_INFO data_ecnt;
    SATA_PHY_ERR_CNT_INFO d2h_data_ecnt;
    SATA_PHY_ERR_CNT_INFO h2d_data_ecnt;
    SATA_PHY_ERR_CNT_INFO ndata_ecnt;
    SATA_PHY_ERR_CNT_INFO d2h_ndata_ecnt;
    SATA_PHY_ERR_CNT_INFO h2d_ndata_ecnt;
} SATA_PHY_EVENT_INFO;
typedef struct _SAS_PHY_ERR_LOG
{
    uint32_t invd_dw_cnt;
    uint32_t runn_disp_err_cnt;
    uint32_t loss_dw_sync_cnt;
    uint32_t phy_rst_prbm_cnt;
} SAS_PHY_ERR_LOG;
typedef union _PHY_ERR_CNT_INFO
{
    SATA_PHY_EVENT_INFO sata;
    SAS_PHY_ERR_LOG sas;
} PHY_ERR_CNT_INFO;

/*@}*/  /* end of physical disk module */

/**
 * @defgroup HAL_ENCLOSURE Storage Enclosure function
 */

/**
 * @ingroup HAL_ENCLOSURE Storage Enclosure function
 */
/*@{*/  /* begin of storage enclosure module */

#define SE_CAP_REDUNDANT_PWR    0x00000001      /*!< Redundant power support. */
#define SE_CAP_CASE_INTRUSION   0x00000002      /*!< Case intrusion support. */
#define SE_CAP_PWR_LAST_STATE   0x00000004      /*!< Keep power last state. */
#define SE_CAP_EUP              0x00000008      /*!< Energy-Using Product support. */
#define SE_CAP_CPU_TEMP         0x00000010      /*!< CPU Temperateure support. */
#define SE_CAP_FAN_MODULE       0x00000020      /*!< Fan module support. */
#define SE_CAP_SUSPEND_RAM      0x00000040      /*!< S3 resume support. */
#define SE_CAP_NO_SAS_ENCLOSURE 0x00000080      /*!< SAS enclosure not support. */
#define SE_CAP_USB_BUTTON       0x00000100      /*!< USB button support. */
#define SE_CAP_NO_ESATA         0x00000200      /*!< ESATA not support. */
#define SE_CAP_INTERNAL_EXP     0x00000400      /*!< Internal combine a SAS expander */
#define SE_CAP_CONTROL_HDD_LED  0x00000800      /*!< Control HDD led support */
#define SE_CAP_CPU_FREQ_ADAPTIVE 0x00001000     /*!< CPU frequency adaptive support */
#define SE_CAP_10G_LED          0x00002000      /*!< 10G led support */
#define SE_CAP_LCM_MODULE       0x00004000      /*!< LCM module suport */
#define SE_CAP_EXTEND_PCIE      0x00008000      /*!< Extend PCIE support */
#define SE_CAP_MCU              0x00010000      /*!< MCU process support*/
#define SE_CAP_NO_HOTSWAP       0x00020000      /*!< Internal disk does not has hot swap capability */
#define SE_CAP_CONTROL_PD_WAKEUP 0x00040000     /*!< Not to wakeup all standby disks support */
#define SE_CAP_MMC_ENCLOSURE    0x00080000      /*!< Support Memory Card , like SD or eMMC */
#define SE_CAP_USB_ENCLOSURE    0x00100000      /*!< Support USB enclosure*/
#define SE_CAP_ODD_ENCLOSURE    0x00200000      /*!< Support ODD enclosure*/
#define SE_CAP_PIC              0x00400000      /*!< PIC support*/
#define SE_CAP_THUNDERBOLT      0x00800000      /*!< Thunderbolt HBA support*/
#define SE_CAP_NO_FAN_READ      0x01000000      /*!< FAN read not support.*/
#define SE_CAP_HDMI		        0x02000000      /*!< HDMI output support.*/
#define SE_CAP_IR		        0x04000000      /*!< Infrared support.*/
#define SE_CAP_CACHE_ENCLOSURE	0x08000000      /*!< Cache enclosure support.*/
#define SE_CAP_AUDIO_JACK	    0x10000000      /*!< Audio Jack support.*/
#define SE_CAP_BBU              0x20000000      /*!< BBU support.*/
#define SE_CAP_EXP_LED          0x40000000      /*!< Expansion led support.*/
#define SE_CAP_MONITOR          0x80000000      /*!< Monitor FAN control, PD standby, and PD smart info*/

/* Extended enclosure capabilities bit definition */
#define SE_EXT_CAP_EJT_BUTTON	0x00000001      /*!< Eject button support. */
#define SE_EXT_CAP_HW_RAID      0x00000002      /*!< TR. External H/W raid device. */
#define SE_EXT_CAP_FW_UPDATE    0x00000004      /*!< Support f/w update. */
#define SE_EXT_CAP_T_PAIR       0x00000008      /*!< Active T-pair */

/* QNAP Turbo Raid capabilities bit definition*/
#define TR_CAP_RAID_HW_CONTROL          0x00000001
#define TR_CAP_RAID_SW_CONTROL          0x00000002
#define TR_CAP_GET_PORT_SATAUS          0x00000004
#define TR_CAP_MULTI_RAID               0x00000008
#define TR_CAP_RAID_GET_INFO            0x00000010
#define TR_CAP_RAID_SPARE_OPERATION     0x00000020
#define TR_CAP_RAID_SYNC                0x00000040
#define TR_CAP_RAID_SCRUB               0x00000080
#define TR_CAP_RAID_ADJUST_SYNC_PRIO    0x00000100
#define TR_CAP_RAID_MIGRATE             0x00000200
#define TR_CAP_RAID_LOCATE              0x00000400

/* QNAP Turbo Raid raid type definition*/
#define TR_RAID_MODE_JBOD               0x00000001
#define TR_RAID_MODE_RAID0              0x00000002
#define TR_RAID_MODE_RAID1              0x00000004
#define TR_RAID_MODE_RAID5              0x00000008
#define TR_RAID_MODE_RAID6              0x00000010
#define TR_RAID_MODE_RAID10             0x00000020
#define TR_RAID_MODE_RAID50             0x00000040
#define TR_RAID_MODE_RAID60             0x00000080

/**
 * @fn          typedef int (*SE_ENUM_CALLBACK) (int enc_id, void* contextP)
 * @brief       Enclosure enumeration callback function prototype.
 * @param[in]   enc_id Enclosure identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 * @retval      -2 Skip, callback function want to skip this identifier.
 */
typedef int (*SE_ENUM_CALLBACK) (int enc_id, void* contextP);

#ifdef STORAGE_V2
/**
 * @fn          int get_enclosure_list_callback(int enc_id, void *contextP)
 * @brief       Enclosure enumeration callback function prototype.
 * @param[in]   enc_id Enclosure identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 * @retval      -2 Skip, callback function want to skip this identifier.
 */
int get_enclosure_list_callback(int enc_id, void *contextP);
#endif

/**
 * @enum        ENCLOSURE_PROTOCOL
 * @brief       Enclosure access protocol.
 */
typedef enum _ENCLOSURE_PROTOCOL
{
    PROTOCOL_UNKNOWN = -1,              /*!< Unknown protocol */
    PROTOCOL_INTERNAL = 0,              /*!< The NAS itself. */
    PROTOCOL_SATA,                      /*!< SATA port multiplier enclosure. */
    PROTOCOL_USB,                       /*!< USB hub. */
    PROTOCOL_SAS,                       /*!< SAS expander enclosure. */
    PROTOCOL_ISCSI,                     /*!< iSCSI enclosure */
    PROTOCOL_PCIE,                      /*!< PCIE enclosure */
    PROTOCOL_THUNDERBOLT,               /*!< Thunderbolt enclosure */
    PROTOCOL_MMC,                       /*!< Multimedia Card enclosure */
    PROTOCOL_ODD,                       /*!< Optical Disk Drive enclosure */
    PROTOCOL_QM2,                       /*!< PCIE enclosuare special case */
    PROTOCOL_USB_RAID,                  /*!< Usb Turbo Raid */
    PROTOCOL_THUNDERBOLT_RAID,          /*!< Thunderbolt Turbo Raid enclosure */
    PROTOCOL_QXP,                       /*!< QXP enclosuare */
    PROTOCOL_TL_SATA,                   /*!< TL SATA enclosuare */
} ENCLOSURE_PROTOCOL;

/**
 * @enum        ENC_TYPE
 * @brief       Enclosure type.
 */
typedef enum _ENC_TYPE
{
    ENC_TYPE_UNKNOWN = -1,
    ENC_TYPE_HOST = 0,
    ENC_TYPE_REXP,
    ENC_TYPE_VJBOD,
    ENC_TYPE_TR_JBOD,
} ENC_TYPE;

/**
 * @enum        SAS_LINK_SPEED
 * @brief       Link speed of the connected SAS JBOD.
 */
typedef enum _SAS_LINK_SPEED
{
    SAS_LINK_SPEED_UNKNOWN = 0,           /* PHY Loss       */
    SAS_LINK_SPEED_1_5_G   = 1,           /* Link Rate 1.5G */
    SAS_LINK_SPEED_3_0_G   = 2,           /* Link Rate 3G   */
    SAS_LINK_SPEED_6_0_G   = 3,           /* Link Rate 6G   */
    SAS_LINK_SPEED_12_0_G  = 4,           /* Link Rate 12G  */
} SAS_LINK_SPEED;

#define GET_SAS_ADDR_NAA(addr) \
            (addr & 0xF000000000000000) >> 60
#define GET_SAS_ADDR_IEEE_CO(addr) \
            (addr & 0x0FFFFFF000000000) >> 36
#define GET_SAS_ADDR_VSI(addr) \
            (addr & 0x0000000FFFFFFFFF)

typedef enum _SAS_CONTAINER_TYPE
{
    SAS_TYPE_UNKNOW = 0,
    SAS_TYPE_HBA,
    SAS_TYPE_EXP,
    SAS_TYPE_PD,
} SAS_CNTR_TYPE;

/**
 * @enum        SAS_PHY_ROUTE
 * @brief       Phy route method of expander.
 */
typedef enum _SAS_PHY_ROUTE
{
    PR_D = 'D',               /*!< Direct routing. */
    PR_S = 'S',               /*!< Subtractive routing. */
    PR_T = 'T',               /*!< Table routing. */
    PR_U = 'U',               /*!< Table to table routing supported. */
    PR_END = 0xFF,            /*!< End of phy routing type. */
} SAS_PHY_ROUTE;

/**
 * @enum        SAS_PHY_STATUS
 * @brief       Phy status of expander.
 */
typedef enum _SAS_PHY_STATUS
{
    PS_DIS = 0,             /*!< Disabled. */
    PS_RST,                 /*!< Reset problem. */
    PS_HOLD,                /*!< Spinup hold. */
    PS_PS,                  /*!< Port selector. */
    PS_RIP,                 /*!< Reset in progress. */
    PS_USPA,                /*!< Unsupported phy attached. */
    PS_ATCH,                /*!< Attached. */
    PS_END,                 /*!< End of phy status. */
} SAS_PHY_STATUS;

/**
 * @enum        SAS_PHY_DEV_TYPE
 * @brief       Phy device type of expander.
 */
typedef enum _SAS_PHY_DEV_TYPE
{
    PDT_EXP = 'e',          /*!< Expander. */
    PDT_FEX = 'f',          /*!< Fanout expander. */
    PDT_RES = 'U',          /*!< Unknown attached device type. */
    PDT_V = 'V',            /*!< Virtual phy. */
    PDT_I = 'i',            /*!< Initiator protocol. */
    PDT_T = 't',            /*!< Target protocol. */
    PDT_END = 0xFF,         /*!< End of phy attached device type. */
} SAS_PHY_DEV_TYPE;

/**
 * @enum        ATT_DEV_INFO_TYPE
 * @brief       Attached device information type.
 */
typedef enum _ATT_DEV_INFO_TYPE
{
    ATT_TYPE_UNKNOW = 0,            /*!< Unknow attached dev type. */
    ATT_TYPE_SATA_PHY_EVENT,        /*!< SATA phy event. */
    ATT_TYPE_UCHAR_ARRAY,           /*!< Character array. */
    ATT_TYPE_SAS_DRV_PHY_ERR_LOG,   /*!< Phy error log of sas drive. */
    ATT_TYPE_SAS_PORT,              /*!< SAS port of expander. */
    ATT_TYPE_SAS_HBA_CARD,          /*!< SAS HBA card. */
} ATT_DEV_INFO_TYPE;

// Forward declaration
typedef struct _SAS_PORT_INFO SAS_PORT_INFO;
typedef struct _SAS_HBA_INFO SAS_HBA_INFO;

/**
 * @struct      ATT_DEV_INFO
 * @brief       Attached device information.
 */
typedef struct _ATT_DEV_INFO
{
    ATT_DEV_INFO_TYPE type;         /*!< Attached device type. */
    union
    {
        SATA_PHY_EVENT_INFO sata_phy_event_info;    /*!< sata phy event. */
        SAS_PHY_ERR_LOG sas_phy_err_log;/*!< SAS phy error log. */
        SAS_PORT_INFO *sas_port_info;   /*!< Pointer of sas port. */
        SAS_HBA_INFO *sas_hba_info;     /*!< Pointer of sas HBA card. */
    } info;                             /*!< Attached device info. */
} ATT_DEV_INFO;

/**
 * @struct      SAS_PHY_INFO
 * @brief       Storage Phy Inforamation of EXP.
 */
typedef struct _SAS_PHY_INFO
{
    uint64_t my_sas_addr;       /*!< This phy sas address. */
    uint32_t phy_id;            /*!< This phy identifier. */
    SAS_PHY_ROUTE r_type;       /*!< Routing type. */
    SAS_PHY_STATUS phy_stat;    /*!< Phy status. */
    uint64_t att_sas_addr;      /*!< Attached device's sas addr. */
    uint32_t att_phy_id;        /*!< Attached device's phy id. */
    uint16_t dev_handle;        /*!< Device handel (HBA card used). */
    uint16_t att_dev_handle;    /*!< Attached Device handel (HBA card used). */
    SAS_PHY_DEV_TYPE dev_type;  /*!< Attached device type. */
    uint8_t belong_port_id;     /*!< Belong port id. */
#define DEV_PTC_SUP_PORT_SEL(cap) \
            (cap & 0x01)
#define DEV_PTC_SUP_SSP(cap) \
            (cap & 0x02)
#define DEV_PTC_SUP_STP(cap) \
            (cap & 0x04)
#define DEV_PTC_SUP_SMP(cap) \
            (cap & 0x08)
#define DEV_PTC_SUP_SATA(cap) \
            (cap & 0x10)
#define DEV_PTC_EDIT_PORT_SEL(cap, stat) \
            if (stat) \
                cap |= 0x01; \
            else \
                cap &= ~0x01;
#define DEV_PTC_EDIT_SSP(cap, stat) \
            if (stat) \
                cap |= 0x02; \
            else \
                cap &= ~0x02;
#define DEV_PTC_EDIT_STP(cap, stat) \
            if (stat) \
                cap |= 0x04; \
            else \
                cap &= ~0x04;
#define DEV_PTC_EDIT_SMP(cap, stat) \
            if (stat) \
                cap |= 0x08; \
            else \
                cap &= ~0x08;
#define DEV_PTC_EDIT_SATA(cap, stat) \
            if (stat) \
                cap |= 0x10; \
            else \
                cap &= ~0x10;
    // TODO: review virtual phy whether suitable using below parameter
    uint8_t dev_ptc_sup;        /*!< A bitmap,
                                     Records device's protocol supported.
                                     bit 0 : PORT_SEL
                                     bit 1 : SSP
                                     bit 2 : STP
                                     bit 3 : SMP
                                     bit 4 : SATA
                                    */
    double link_rate;           /*!< Phy link rate. */
    ATT_DEV_INFO att_dev_info;  /*!< Attached device info. */
    // discover attributes
    int exp_chag_cnt;           /*!< Expander change count. */
    uint8_t att_br_cap;         /*!< Attached break_reply capable. */
    uint64_t att_dev_name;      /*!< dev_name (wwn or addr etc..). */
    uint8_t att_dev_type;       /*!< Attached device type. */
    uint8_t att_per_cap;        /*!< Attached persistent capable. */
    uint8_t att_iz_per;         /*!< Attached inside ZPSDS persistent. */
    uint8_t att_pa_cap;         /*!< Attached partial capable. */
    uint8_t att_pow_cap;        /*!< Attached power capable. */
    uint8_t att_pwr_dis_cap;    /*!< Attached pwr_dis capable. */
    uint8_t att_reason;         /*!< Status of attached. */
    uint8_t att_req_iz;         /*!< Attached requested inside ZPSDS. */
    uint8_t att_sata_dev;       /*!< Attached sata device. */
    uint8_t att_sata_host;      /*!< Attached sata host. */
    uint8_t att_sata_ps;        /*!< Attached sata port selector. */
    uint8_t att_sl_cap;         /*!< Attached slumber capable. */
    uint8_t att_smp_init;       /*!< Attached initiator. */
    uint8_t att_smp_targ;       /*!< Attached target. */
    uint8_t att_ssp_init;       /*!< Attached initiator. */
    uint8_t att_ssp_targ;       /*!< Attached target. */
    uint8_t att_stp_init;       /*!< Attached initiator. */
    uint8_t att_stp_targ;       /*!< Attached target. */
    uint8_t conn_elem_ind;      /*!< Connector element index, base on 0 */
    uint8_t conn_p_link;        /*!< Connector physical link. */
    uint8_t conn_type;          /*!< Connector type. */
    uint8_t dev_slot_num;       /*!< Device slot number. */
    uint8_t dev_slot_grp_num;   /*!< Device slot group number. */
#define MAX_DEV_SLOT_GRP_OUT_CONT 6 /*!< Hard code to 6 (refer smp util). */
    char dev_slot_grp_out_cont[MAX_DEV_SLOT_GRP_OUT_CONT];
                                /*!< Device slot group output connector. */
    uint8_t hw_max_p_lrate;     /*!< Hardware maximum physical link rate. */
    uint8_t hw_min_p_lrate;     /*!< Hardware minimum physical link rate. */
    uint8_t hw_mux_sup;         /*!< Hardware muxing supported. */
    uint8_t iz;                 /*!< Inside ZPSDS. */
    uint8_t iz_pers;            /*!< Inside ZPSDS persistent. */
    uint8_t neg_log_lrate;      /*!< Negotiate link rate or status. */
    uint8_t neg_phy_lrate;      /*!< Negotiate physical link rate. */
    uint8_t opt_m_en;           /*!< Optical mode enabled. */
    uint8_t phy_cc;             /*!< Phy change count. */
    uint8_t phy_power_cond;     /*!< Phy power condition. */
    uint8_t pp_timeout;         /*!< Partial pathway timeout value. */
    uint8_t pr_max_p_lrate;     /*!< Programmed maximum physical link rate. */
    uint8_t pr_min_p_lrate;     /*!< Programmed minimum physical link rate. */
    uint8_t pwr_dis_ctl_cap;    /*!< Pwr_dis control capable. */
    uint8_t pwr_dis_sig;        /*!< Pwr_dis signal. */
    uint8_t reason;             /*!< Reason. */
    uint8_t req_iz;             /*!< Requested inside ZPSDS. */
    uint8_t req_iz_cbe;         /*!< Requested inside ZPSDS changed by exp. */
    uint8_t routing_attr;       /*!< Routing type. */
    uint8_t sas_pa_cap;         /*!< SAS partial capable. */
    uint8_t sas_pa_en;          /*!< SAS partial enable. */
    uint8_t sas_pow_cap;        /*!< SAS power capable. */
    uint8_t sas_sl_cap;         /*!< SAS slumber capable. */
    uint8_t sas_sl_en;          /*!< SAS slumber enabled. */
    uint8_t sata_pa_cap;        /*!< SAS partial capable. */
    uint8_t sata_pa_en;         /*!< SATA partial enable. */
    uint8_t sata_sl_cap;        /*!< SATA slumber capable. */
    uint8_t sata_sl_en;         /*!< SATA slumber enable. */
    uint8_t stp_buff_tsmall;    /*!< STP buffer too small. */
    uint16_t stp_buff_size;     /*!< STP buffer size. */
    uint8_t virt_phy;           /*!< Virtual phy. */
    uint8_t zg;                 /*!< Zone group. */
    uint8_t zg_pers;            /*!< Zone group persistent. */
    uint8_t zoning_en;          /*!< zone group. */
    uint8_t self_conf_status;   /*!< Self-configuration status. */
    uint8_t self_conf_lev_comp; /*!< Self-configuration levels completed. */
    uint64_t self_conf_sas_addr;/*!< Self-configuration sas address. */
    uint32_t prog_phy_cap;      /*!< Programmed phy capabilities. */
    uint32_t curr_phy_cap;      /*!< Current phy capabilities. */
    uint32_t att_phy_cap;       /*!< Attached phy capabilities. */
    uint8_t def_ins_ZPSDS_per;  /*!< Default inside ZPSDS persistent. */
    uint8_t def_req_ins_ZPSDS;  /*!< Default requested inside ZPSDS. */
    uint8_t def_zone_group_per; /*!< Default zone group persistent. */
    uint8_t def_zoning_en;      /*!< Default zoning enabled. */
    uint8_t def_zone_gup;       /*!< Default zone group. */
    uint8_t saved_ins_ZPSDS_per;/*!< Saved inside ZPSDS persistent. */
    uint8_t saved_req_ind_ZPSDS;/*!< Saved requested inside ZPSDS. */
    uint8_t saved_zone_gup_per; /*!< Saved zone group persistent. */
    uint8_t saved_zoning_en;    /*!< Saved zoning enabled. */
    uint8_t saved_zone_gup;     /*!< Saved zone group. */
    uint8_t sdw_ins_ZPSDS_per;  /*!< Shadow inside ZPSDS persistent. */
    uint8_t sdw_req_ins_ZPSDS;  /*!< Shadow requested inside ZPSDS. */
    uint8_t sdw_zone_gup_per;   /*!< Shadow zone group persistent. */
    uint8_t sdw_zoning_en;      /*!< Shadow zoning enabled. */
    uint8_t sdw_zone_gup;       /*!< Shadow zone group. */
    // report phy error log attribute
    SAS_PHY_ERR_LOG err_log;    /*!< SAS phy error log. */
} SAS_PHY_INFO;

/**
 * @struct      SAS_PORT_INFO
 * @brief       Storage Port Inforamation of EXP.
 */
struct _SAS_PORT_INFO
{
    int att_enc_id;             /*!< Attached enclosure's ID. */
    uint32_t port_id;           /*!< This port identifier. */
    uint32_t phys_port_id;      /*!< The physical id (HBA used). */
    uint32_t att_port_id;       /*!< Attached port id. */
    uint64_t att_sas_addr;      /*!< Attached enclosure's sas addr. */
    uint8_t max_phy_num;        /*!< This port phy numbers. */
    uint8_t upstream;           /*!< 0 is downstream. */
    uint64_t sas_addr;          /*!< Sas address of this port. */
    ATT_DEV_INFO att_dev_info;  /*!< Attached device info. */
    SAS_PHY_INFO phy_info[MAX_PHY_NUM_PER_PORT];
                                /*!< Phy info of this port. */
};

/**
 * @struct      SAS_HBA_INFO
 * @brief       SAS HBA Information.
 */
struct _SAS_HBA_INFO
{
    int port_id;                        /*!< Port id. */
    int scsi_host;                      /*!< SCSI host number. */
    int stay_enc_id;                    /*!< Stay in which enclosure ID. */
    int slot_id;                        /*!< Slot ID (0 -> internal. */
    int internal_id;                    /*!< Internal id. */
    int bus;                            /*!< PCI bus. */
    int dev;                            /*!< PCI device. */
    int func;                           /*!< PCI funcion. */
    char board_name[MAX_MODEL_LEN];     /*!< Board name. */
    char board_sn[MAX_SAS_HBA_SN];      /*!< Board sn. */
    char fw_ver[MAX_REVISION_LEN];      /*!< The version of HBA FW. */
    char bios_ver[MAX_REVISION_LEN];    /*!< The version of HBA bios. */
    char wwn[MAX_WWN_LEN];              /*!< World Wide Name of HBA. */
    uint64_t sas_addr;                  /*!< Self sas addr. */
    uint32_t max_phy_num;               /*!< Total phy numbers. */
    SAS_PHY_INFO phy_info[MAX_PHY_NUM_OF_HBA];
                                        /*!< Phys information. */
    uint32_t max_port_num;              /*!< The maximum port numbers. */
    SAS_PORT_INFO port_info[MAX_PORT_NUM_OF_HBA];
                                        /*!< Port info. */
};

/**
 * @struct      EXP_EXT_INFO
 * @brief       Storage EXP Information.
 */
typedef struct _EXP_EXT_INFO
{
    int enc_id;                         /*!< Enclosure id. */
    char sg_name[MAX_SYS_ID_LEN];       /*!< The sg name of expander. */
    char bsg_name[MAX_SYS_ID_LEN];      /*!< The bsg name of expander. */
    char mdl_name[MAX_MODEL_LEN];       /*!< The model name of expander. */
    char ver[MAX_REVISION_LEN];         /*!< The version of expander. */
    uint64_t enc_sas_addr;              /*!< This enclosure sas addr. */
    uint32_t max_phy_num;               /*!< Total phy numbers. */
    SAS_PHY_INFO phy_info[MAX_PHY_NUM_OF_REXP];
                                        /*!< Phys information. */
    uint32_t max_port_num;              /*!< The maximum port numbers. */
    SAS_PORT_INFO port_info[MAX_PORT_NUM_OF_REXP];
                                        /*!< Port info. */
} EXP_EXT_INFO;

/**
 * @struct      ENCLOSURE_INFO
 * @brief       Storage Enclosure Information.
 */
typedef struct _ENCLOSURE_INFO {
    int enc_id;                         /*!< 0 based logical index generated automatically by HAL uppper layer. */
    int enc_stack_id;			/*!< The enclosure stack position related to the root enclosure. */
    char enc_parent_sys_id[MAX_SYS_ID_LEN];     /*!< System dependent enclosure parent identifier when communicating with the system dependent layer. */
    int  enc_parent_port_id;            /*!< System dependent enclosure parent identifier port id when communicating with the system dependent layer. Only for usb enclosure */
    char enc_sys_id[MAX_SYS_ID_LEN];    /*!< System dependent enclosure identifier when communicating with the system dependent layer. */
    char real_sys_id[MAX_SYS_ID_LEN];   /*!< Real sys id (sg number)*/
    char bsg_name[MAX_SYS_ID_LEN];      /*!< SAS bsg name */
    char wwn[MAX_WWN_LEN];              /*!< World Wide Name for the enclousre. */
    char vendor[MAX_VENDOR_LEN];        /*!< Vendor name. */
    char model[MAX_MODEL_LEN];          /*!< Model name. */
    char serial_no[MAX_SERIAL_NO_LEN];  /*!< Enclosure serial number. */
    char revision[MAX_REVISION_LEN];    /*!< Enclosure revision.*/
    // TODO: add below parameter into EXP_EXT_INFO
    int conn_status[MAX_EXT_CONNECTOR_NUM];	/*!< External connector status, for SAS JBOD only .*/
    EXP_EXT_INFO exp_ext_info;          /*!< Extension info of rexp. */
    int status;                         /*!< Current enclosure status. */
    int capabilities;                   /*!< Enclosure capabilities. */
    int capabilities_ext;               /*!< Extended enclosure capabilities. */
    int tr_capabilities;                /*!< TR capabilities.(QNAP Turbo Raid) */
    int tr_supported_raid_mode;         /*!< TR supported raid mode.(QNAP Turbo Raid) */
    ENCLOSURE_PROTOCOL protocol;        /*!< Storage communication protocol. */
    ENC_TYPE enc_type;                  /*!< Enclosure type. */
    unsigned int max_disk_num;          /*!< Maximum number of Disk slots. */
    unsigned int max_sys_fan_region;    /*!< Maximum system fan region counts. */
    unsigned int max_sys_temp_region;   /*!< Maximum number of Region Temperature. */
    unsigned int max_fan_num;           /*!< Maximum number of FAN counts, exclude PSU */
    unsigned int max_cpu_fan_num;       /*!< Maximum number of CPU FAN counts*/
    unsigned int default_cpu_fan_count; /*!< Default CPU FAN counts*/
    unsigned int factory_cpu_fan_count; /*!< Factory FAN counts*/
    unsigned int max_sys_extra_region_count;   /*!< Extra region count of the system, exclude CPU FAN & System FAN */
    unsigned int max_sys_extra_region_fan_num[MAX_EXTRA_FAN_REGION_COUNT];    /*!< Array of FAN counts for all fan regions, exclude CPU FAN & System FAN */
    unsigned int max_temp_num;          /*!< Maximum number of Temperature sensors, exclude PSU. */
    unsigned int max_pcie_slot;         /*!< Maximum number of Extend PCIE slots. */
    unsigned int max_mb_nic_count;      /*!< Maximum number of Onboard NIC counts. */
    unsigned int max_conn_count;        /*!< Maximum number of external connector count. */
    unsigned int max_ujbod_num;         /*! < Limit counts if USB JBOD support. */
    unsigned int max_cpu_num;           /*! < Maximum number of CPU support  */
    //  SWchen Support cache port api 20131104
    int cache_port_bitmap;          /*!< Port bitmap for SSD cache support. */
    //  SWChen 20131213 SS profile support
    int ss_max_channels;
    int ss_free_channels;
    SAS_LINK_SPEED link_speed;
    int tr_bus_speed;
} ENCLOSURE_INFO;

#define MAX_EXT_PORT_NUM   4

/**
 * @struct      EXT_PORT
 * @brief       The External Port Information.
 */
typedef struct _EXT_PORT {
    int attached;
    int ext_port_id;
    int attached_enc_id;
    int reserved;
} EXT_PORT;

/**
 * @struct      EXT_PORT_INFO
 * @brief       All External Ports Information.
 */
typedef struct _EXT_PORT_INFO {
    int max_ext_port_num;
    int pcie_slot;
    EXT_PORT ext_port[MAX_EXT_PORT_NUM];
} EXT_PORT_INFO;

/**
 * @enum        SE_DISK_POWER_STATUS
 * @brief       Storage Enclosure power status.
 */
typedef enum _SE_DISK_POWER_STATUS
{
    SE_DISK_STATUS_STANDBY = 0,               /*!< Standby mode. */
    SE_DISK_STATUS_ACTIVE,                    /*!< Active mode. */
} SE_DISK_POWER_STATUS;

/**
 * @enum        PWR_STATUS
 * @brief       Power status.
 */
typedef enum _PWR_STATUS
{
    PWR_INVALID = -3,           /*!< Power detect is disable. */
    PWR_REMOVE = -2,            /*!< Power module remove status. */
    PWR_FAIL = -1,              /*!< Power failure status. */
    PWR_GOOD = 0,               /*!< Power good status. */
} PWR_STATUS;

/**
 * @enum        FAN_STATUS
 * @brief       Fan status.
 */
typedef enum _FAN_STATUS
{
    FAN_INVALID = -2,            /*!< Fan can't get status. */
    FAN_FAIL = -1,              /*!< Fan failure status. */
    FAN_GOOD = 0,                /*!< Fan good status. */
} FAN_STATUS;

/**
 * @enum        FAN_REGION
 * @brief       FAN control region index.
 */
typedef enum _FAN_REGION
{
    FAN_REG_SYSTEM = 0, /*!< System FAN region index. */
    FAN_REG_CPU,        /*!< CPU FAN region index. */
    FAN_REG_1,          /*!< REGION1 FAN region index. */
    FAN_REG_2,          /*!< REGION2 FAN region index. */
    FAN_REG_NUM,        /*!< The number of FAN region. */
} FAN_REGION;

/**
 * @enum        TEMP_REGION
 * @brief       Temp control region index.
 */
typedef enum _TEMP_REGION
{
    TEMP_REG_SYSTEM = 0, /*!< System Temp region index. */
    TEMP_REG_1,          /*!< REGION1 Temp region index. */
    TEMP_REG_2,          /*!< REGION2 Temp region index. */
    TEMP_REG_NUM,        /*!< The number of Temp region. */
} TEMP_REGION;

/**
 * @enum        REGION_TYPE
 * @brief       Enumeration of all the targets that FAN works for.
 */
typedef enum _REGION_TYPE
{
    REGION_TYPE_INVALID = -1,   /*!< Invalid REGION FAN type. */
    REGION_TYPE_SYS = 0,        /*!< REGION FANs work for SYS. */
    REGION_TYPE_CPU,            /*!< REGION FANs work for CPU. */
    REGION_TYPE_PSU,            /*!< REGION FANs work for PSU. */
    REGION_TYPE_PCIE,           /*!< REGION FANs work for PCIE. */
    REGION_TYPE_NUM,            /*!< The number of FAN target. */
} REGION_TYPE;

typedef enum _HEAT_SOURCE
{
    HEAT_SYS = 0,       /*!< System temperature. */
    HEAT_CPU,           /*!< CPU temperature. */
    HEAT_DISK,          /*!< Disk temperature. */
    HEAT_NUM            /*!< The number of heat source. */
} HEAT_SOURCE;

typedef enum _TFAN_MODE
{
    TFAN_MODE_NORMAL = 0,      /*!< Tfan, normal mode. */
    TFAN_MODE_QUIET,             /*!< Tfan, quiet mode. */
    TFAN_MODE_PERFORMANCE,  /*!< Tfan, performance mode. */
} TFAN_MODE;

/**
 * @struct      ENCLOSURE_SYS_STATUS
 * @brief       Storage Enclosure system status.
 */
typedef struct _ENCLOSURE_SYS_STATUS {
    int cpu_temp;                               /*!< CPU average temperature, -1 if not exist */
    int cpu_count;                              /*!< CPU count */
    int cpu_s_temp[MAX_EM_INFO_CPU_COUNT];      /*!< CPU individual temperature */
    int sys_temp;                               /*!< System Temperature, -1 if not exist */
    int sys_region_temp[MAX_EXTRA_TEMP_REGION_COUNT]; /*!< Region Temperature */
    int region_temp_count;                      /*!< Total temp region count . */
    int env_count;                              /*!< Environment Thermal sensor, -1 if not exist */
    int env_temp[MAX_EM_INFO_ENV_COUNT];        /*!< Environment Temperature . */
    int fan_count;                              /*!< Fan count . */
    int cpu_fan_count;                          /*!< CPU Fan count . */
    int default_cpu_fan_count;                  /*!< Default CPU Fan count . */
    int factory_cpu_fan_count;                  /*!< Default CPU Fan count . */
    int extra_fan_region_count;                 /*!< Extra fan region count . */
    int region_fan_count[MAX_EXTRA_FAN_REGION_COUNT];                          /*!< Fan count for each system region. */
    FAN_STATUS fan_status[MAX_EM_INFO_FAN_COUNT];    /*!< Fan status . */
    FAN_STATUS cpu_fan_status[MAX_EM_INFO_FAN_COUNT];    /*!< CPU Fan status . */
    FAN_STATUS region_fan_status[MAX_EXTRA_FAN_REGION_COUNT][MAX_EM_INFO_FAN_COUNT];    /*!< System Region Fan status . */
    int fan_rpm[MAX_EM_INFO_FAN_COUNT];         /*!< Fan rpm . */
    int cpu_fan_rpm[MAX_EM_INFO_FAN_COUNT];         /*!< CPU Fan rpm . */
    int region_fan_rpm[MAX_EXTRA_FAN_REGION_COUNT][MAX_EM_INFO_FAN_COUNT];         /*!< System Region Fan rpm . */
    int pwr_count;                              /*!< Power supply count . */
    PWR_STATUS pwr_status[MAX_EM_INFO_PWR_COUNT];    /*!< Power status . */
    int pwr_temp[MAX_EM_INFO_PWR_COUNT];        /*!< Power Temperature . */
    int pwr_rpm[MAX_EM_INFO_PWR_COUNT];         /*!< Power rpm . */
    int switch_mode;
} ENCLOSURE_SYS_STATUS;

/**
 * @enum        BUZZER_MODE
 * @brief       System buzzer mode.
 */
typedef enum _BUZZER_MODE
{
    BUZZER_SHORT = 0,                  /*!< period 0.5 sec . */
    BUZZER_LONG,                       /*!< period 1.5 sec . */
    BUZZER_USB_COPY_ERROR,
    BUZZER_RAID_DEGRADE,
    BUZZER_HD_PLUGIN,
    BUZZER_HD_PLUGOUT,
    BUZZER_RAID_SYNC_START,
    BUZZER_RAID_SYNC_END,
    BUZZER_FAN_ERROR,
    BUZZER_POWER_ERROR,
    BUZZER_VOLUME_ALMOST_FULL,
    BUZZER_VOLUME_FULL,
    BUZZER_JBOD_PLUGIN,
    BUZZER_JBOD_PLUGOUT,
    BUZZER_HD_ERROR,
#ifdef STORAGE_V2
    BUZZER_POOL_HIT_THRESHOLD,
    BUZZER_POOL_FULL,
    BUZZER_POOL_STATUS_ERROR,
    BUZZER_VOLUME_HIT_THRESHOLD,
    BUZZER_VOLUME_STATUS_ERROR,
    BUZZER_ISCSI_HIT_THRESHOLD,
#endif
    BUZZER_SYS_STARTUP = 100,
    BUZZER_SYS_SHUTDOWN = 101,
    BUZZER_NOP,                  /*!< buzzer no operation. */
} BUZZER_MODE;

/**
 * @enum    VOICE_ID
 * @brief     Available system voice list.
 */
typedef enum _VOICE_ID
{
    VOICE_SYSTEM_SHUTDOWN = 0,
    VOICE_SYSTEM_BOOT_DONE,
    VOICE_FW_UPGRADE_START,
    VOICE_FW_UPGRADE_DONE,
    VOICE_STSTEM_RESTART,
    VOICE_ADVANCED_STSTEM_RESTART,
    VOICE_USB_EJECTION,
    VOICE_USB_SAFLY_DETACH,
    VOICE_USB_ONE_TOUCH_COPY,
    VOICE_USB_BACKUP_DONE,
    VOICE_HD_STATION_START,
    VOICE_HD_STATION_RESTART,
    VOICE_LINUX_STATION_START,
    VOICE_LINUX_STATION_RESTART,
    VOICE_AUDIO_TESET,
    VOICE_STORAGE_POOL_HIT_THRESHOLD,
    VOICE_STORAGE_POOL_FULL,
    VOICE_STORAGE_POOL_ERROR,
    VOICE_STORAGE_VOL_HIT_THRESHOLD,
    VOICE_STORAGE_VOL_FULL,
    VOICE_STORAGE_ISCSI_LUN_HIT_THRESHOLD,
    VOICE_JBOD_PLUGIN,
    VOICE_JBOD_PLUGOUT,
    VOICE_HW_POWER_ERROR,
    VOICE_HW_FAN_ERROR,
    VOICE_HDD_ERROR,
    VOICE_HDD_PLUGIN,
    VOICE_HDD_PLUGOUT,
    VOICE_STORAGE_RAID_INIT_REBUILD_START,
    VOICE_STORAGE_RAID_INIT_REBUILD_DONE,
    VOICE_TBT_DEVICE_CONNECT,
    VOICE_TBT_DEVICE_DISCONNECT,
    VOICE_USB_QA_DEVICE_CONNECT,
    VOICE_USB_QA_DEVICE_DISCONNECT,
    VOICE_RAID_DEGRADE,
    VOICE_VOLUME_STATUS_ERROR,
    VOICE_TAKE_SNAPSHOT,
    VOICE_TR_DEVICE_CONNECT,
    VOICE_TR_DEVICE_DISCONNECT,
    VOICE_ID_NUM, /*!< The number of voice id. */
} VOICE_ID;

/**
 * @enum        VOICE_MODE
 * @brief       System voice mode.
 */
typedef enum _VOICE_MODE
{
    AUDIO_VOICE_POOL_HIT_THRESHOLD     = 15,
    AUDIO_VOICE_POOL_FULL              = 16,
    AUDIO_VOICE_POOL_STATUS_ERROR      = 17,
    AUDIO_VOICE_VOLUME_HIT_THRESHOLD   = 18,
    AUDIO_VOICE_VOLUME_FULL            = 19,
    AUDIO_VOICE_ISCSI_HIT_THRESHOLD    = 20,
    AUDIO_VOICE_RAID_SYNC_START        = 28,
    AUDIO_VOICE_RAID_SYNC_END          = 29,
} VOICE_MODE;

int get_nc_app_A002_name(OUT char* buf, IN int buf_size);
int get_nc_app_A049_name(OUT char* buf, IN int buf_size);

/**
 * @fn int SE_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN SE_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available storage enclosures from the internal configuration file.
 *
 * This function is called by nas utility to obtain all available logical
 * enclosure identifiers in the internal configuration file.
 * @param[out] id_ary An identifier array to host the available enclosure identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Enclosure enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >= 0 Success, return the count of the available storage enclosures, if the count
 *         is larger than id_ary_count, the calling utility should allocate more
 *         space for identifier array to obtain all available enclosure
 *         identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int SE_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
                 IN SE_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int SE_Get_Info(IN int enc_id, OUT ENCLOSURE_INFO *enc_infoP)
 * @brief Retrieve the specified enclosure object from the internal configuration file.
 *
 * This function is called by nas utility to obtain the specified enclosure object information
 * from the internal configuration file.
 * @param[in] enc_id logical enclosure identifier.
 * @param[out] enc_infoP a ENCLOSURE_INFO structure pointer to host the enclosure object information.
 * @retval 0 Success, the specified enclosure object is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified enclosure object in the
 *         internal configurationturn file.
 */
int SE_Get_Info(IN int enc_id, OUT ENCLOSURE_INFO *enc_infoP);

/**
 * @fn int SE_Get_Capability(IN int enc_id, OUT int *capabilityP)
 * @brief Retrieve the specified enclosure capability.
 *
 * This function is called by nas utility to obtain the specified enclosure capability.
 * @param[in] enc_id logical enclosure identifier.
 * @param[out] capabilityP The retrived enclosure capability.
 * @retval 0 Success, the specified enclosure capability is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified enclosure capability in the
 *         internal configurationturn file.
 */
int SE_Get_Capability(IN int enc_id, OUT int *capabilityP);

/**
 * @fn int SE_Get_Parent_Sys_Id(IN int enc_id, OUT char* sys_id, IN int buf_len)
 * @brief Retrieve the enclosure parent system identifier from the internal configuration file.
 *
 * @note The parent system identifier of the root enclosure does not exist.
 * @param[in] enc_id logical enclosure identifier.
 * @param[out] sys_id a string buffer to accommodate the output parent identifier.
 * @param[in] buf_len the buffer length of parent system identifier.
 * @retval 0 Success, the specified parent system identifier is retrieved completely.
 * @retval -1 Generic failure, the specified parent system identifier doesn't exist.
 */
int SE_Get_Parent_Sys_Id(IN int enc_id, OUT char* sys_id, IN int buf_len);

/**
 * @fn int SE_Get_Parent_Port_Id(IN int enc_id, OUT int* port_idP)
 * @brief Retrieve the enclosure parent port identifier from the internal configuration file.
 *
 * @note The parent port identifier of the root enclosure does not exist.
 * @param[in] enc_id logical enclosure identifier.
 * @param[out] port_idP a buffer to accommodate the output parent port identifier.
 * @retval 0 Success, the specified parent port identifier is retrieved completely.
 * @retval -1 Generic failure, the specified parent port identifier doesn't exist.
 */
int SE_Get_Parent_Port_Id(IN int enc_id, OUT int* port_idP);

/**
 * @fn int SE_Get_WWN(IN int enc_id, OUT char* wwn, IN int buf_len)
 * @brief Retrieve the enclosure WWN from the internal configuration file.
 *
 * This enclosure WWN might not exist according to the enclosure protocol type.
 * @param[in] enc_id logical enclosure identifier.
 * @param[out] wwn a string buffer to accommodate the output WWN.
 * @param[in] buf_len the wwn buffer length.
 * @retval 0 Success, the specified enclosure WWN is retrieved completely.
 * @retval -1 Generic failure, the specified enclosure WWN doesn't exist.
 */
int SE_Get_WWN(IN int enc_id, OUT char* wwn, IN int buf_len);

/**
 * @fn int SE_Get_Unique_Id(IN ENCLOSURE_INFO *enc_infoP, OUT char* uid_string, IN int buf_len)
 * @brief Retrieve the unique enclosure identifier from the enclosure information.
 *
 * This unique identifier might be a WWN or serial no or doesn't exist according to the
 * enclosure information.
 * @param[in] enc_infoP a ENCLOSURE_INFO structure retrieved from system dependent layer.
 * @param[out] uid_string a string buffer to accommodate the output unique identifier.
 * @param[in] buf_len the uid_string buffer length.
 * @retval 0 Success, the specified enclosure UID is retrieved completely.
 * @retval -1 Generic failure, the specified enclosure UID doesn't exist.
 */
int SE_Get_Unique_Id(IN ENCLOSURE_INFO *enc_infoP, OUT char* uid_string, IN int buf_len);

/**
 * @fn int SE_Export_Enc_Map(void)
 * @brief Export the enclosure uid mapping from hal.conf to enclosure.map.
 *
 * This function is called by NAS disk initialize utility to export the
 * current uid mapping in the /etc/hal.conf to /etc/enclosure.map.
 * @retval 0 Success, the enclosure mapping is exported completely.
 * @retval -1 Generic failure, the enclosure mapping doesn't exist.
 */
int SE_Export_Enc_Map(void);

/**
 * @fn int SE_Import_Enc_Map(void)
 * @brief Import the enclosure uid bitmap from enclosure.map to hal.conf.
 *
 * This function is called by NAS boot utility to import last enclosure id
 * bitmap from /etc/enclosure.map to /etc/hal.conf.
 * @retval 0 Success, the enclosure mapping is imported completely.
 * @retval -1 Generic failure, the enclosure mapping doesn't exist.
 */
int SE_Import_Enc_Map(void);

/**
 * @fn int SE_Release_Id(IN int enc_id)
 * @brief Release the allocated persistent enclosure identifier.
 *
 * This function is called by management utility to release the specified
 * enclosure identifier and bitmap in /etc/enclosure.map.
 * @note After calling this function, the NAS should reboot to rescan and
 *  reassign the identifier.
 * @param[in] enc_id The enclosure identifier that has to be released, -1 represent
 *            to release all persistent enclosure identifiers.
 * @retval 0 Success, the enclosure identifier and bitmap has been released.
 * @retval -1 Generic failure, the enclosure mapping doesn't exist.
 */
int SE_Release_Id(IN int enc_id);

/**
 * @fn int SE_Is_Id_Available(IN int enc_id)
 * @brief Check if the specified identifier is free to be assigned or not.
 *
 * This function is called by management utility to check the specified
 * enclosure identifier is available to be assigned or not.
 * @param[in] enc_id The enclosure identifier that needs to be checked, -1 represent
 *            to check all enclosure identifiers.
 * @retval 1 True, the specified enclosure identifier is free and could be assigned.
 * @retval 0 False, the specified enclosure identifier is occupied.
 */
int SE_Is_Id_Available(IN int enc_id);

/**
 * @fn ENCLOSURE_PROTOCOL SE_Get_Protocol(IN int enc_id)
 * @brief Retrieve the specified enclosure protocol type from the internal configuration file.
 *
 * This function is called by nas utility to obtain the specified enclosure protocol type
 * from the internal configuration file.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The enclosure protocol type for the specified enclosure object.
 */
ENCLOSURE_PROTOCOL SE_Get_Protocol(IN int enc_id);

/**
 * @fn int SE_Sys_Set_SAS_HBA_Link_Rate(IN char *enc_sys_id, IN int slot_id,
        IN int internal_id, IN int phy, IN PD_LINK_SPEED max,
        IN PD_LINK_SPEED min)
 * @brief Change link rate to specific sas device through sysfs.
 *
 * This function will change link rate of sas device through sysfs.
 * @param[in] enc_sys_id Enclosure sys id.
 * @param[in] slot_id Slot id.
 * @param[in] internal_id Internal id.
 * @param[in] phy_id The phy id.
 * @param[in] max The maximum link rate.
 * @param[in] min The minimum link rate.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Set_SAS_HBA_Link_Rate(IN int enc_id, IN int slot_id,
    IN int internal_id, IN int phy, IN PD_LINK_SPEED max,
    IN PD_LINK_SPEED min);

/**
 * @fn int SE_Clear_SAS_HBA_Phy_Err(IN int enc_id, IN int slot_id,
 *    IN int internal_id, IN int phy);
 * @brief Clear error counts of specific sas hba's phy.
 *
 * This function will clear error counts of specific sas hba's phy
 * @param[in] enc_id Enclosure id.
 * @param[in] slot_id Slot id.
 * @param[in] internal_id Internal id.
 * @param[in] phy_id The phy id.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Clear_SAS_HBA_Phy_Err(IN int enc_id, IN int slot_id,
    IN int internal_id, IN int phy);

/**
 * @fn int SE_Set_SMP_SAS_Link_Rate(IN int enc_id,
 *     IN int phy_id, IN PD_LINK_SPEED max, IN PD_LINE_SPEED min, IN FILE *fd)
 * @brief Change link rate to specific sas device through SMP protocol.
 *
 * This function will issue a smp command to change the link rate.
 * @param[in] enc_id The enclsoure id.
 * @param[in] phy_id The phy id.
 * @param[in] max The maximum link rate.
 * @param[in] min The minimum link rate.
 * @param[in] fd File descriptor for redirecting debug message, def. stderr.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Set_SMP_SAS_Link_Rate(IN int enc_id,
    IN int phy_id, IN PD_LINK_SPEED max, IN PD_LINK_SPEED min, IN FILE *fd);

/**
 * @fn int SE_Clear_SMP_SAS_Phy_Error(IN int enc_id, IN int phy_id, IN FILE *fd)
 * @brief Clear phy error of specific enclosure's expander.
 *
 * This function will issue a smp command to clear phy error of expander.
 * @param[in] enc_id The enclsoure id.
 * @param[in] phy_id The phy id.
 * @param[in] fd File descriptor for redirecting debug message, def. stderr.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Clear_SMP_SAS_Phy_Error(IN int enc_id, IN int phy_id, IN FILE *fd);

/**
 * @fn int SE_Control_SMP_Phy(IN int enc_id, IN int phy_id,
        IN int mode, IN FILE *fd)
 * @brief Control phy through smp protocol.
 *
 * This function will issue a smp command to control phy.
 * @param[in] enc_id The enclsoure id.
 * @param[in] phy_id The phy id.
 * @param[in] mode The control mode.
 * @param[in] fd File descriptor for redirecting debug message, def. stderr.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Control_SMP_Phy(IN int enc_id, IN int phy_id, IN int mode, IN FILE *fd);

/**
 * @fn int SE_Get_Phys_Of_Att_Upstream_Port(IN int enc_id,
 *       OUT SAS_CNTR_TYPE *sas_cntr, OUT int *parent_enc_id,
 *       OUT int *slot_id, OUT int *internal_id,
 *       OUT int *phys_arr, OUT int *num_phy)
 * @brief Get the phys of the port that attached to the specific
        enclosure's upstream.
 *
 * This function will get the phys of the port that attached to the
        specific enclosure's upstream.
 * @param[in] enc_id The enclsoure id.
 * @param[out] sas_cntr The attached port belong to which container.
 * @param[out] parent_enc_id The attached port belong which enclosure.
 * @param[out] bus SAS HBA bus.
 * @param[out] dev SAS HBA dev.
 * @param[out] func SAS HBA func.
 * @param[out] phys_arr This array record phys' id of attached port.
 * @param[inout] num_phy The number of the phys_arr, function will
        edit this value to appear current phy numbers.
 * @retval 0 Success.
 * @retval -1 Failure, this function will fill the parameters even if
        failure status return, don't use parameters if return -1.
 */
int SE_Get_Phys_Of_Att_Upstream_Port(IN int enc_id,
    OUT SAS_CNTR_TYPE *sas_cntr, OUT int *parent_enc_id,
    OUT int *bus, OUT int *dev, OUT int *func,
    OUT int *phys_arr, INOUT int *num_phy);

/**
 * @fn int SE_Discover_Exp(INOUT EXP_EXT_INFO *exp)
 * @brief Discover specific expander.
 *
 * This function will discover specific expander.
 * @param[inout] exp Pointer of expander structure.
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int SE_Discover_Exp(INOUT EXP_EXT_INFO *exp);

/**
 * @fn int SE_Get_LSI_SAS_HBA_Info(OUT SAS_HBA_INFO *sas_hba,
        INOUT int *hba_num, IN FILE *fd)
 * @brief Query LSI SAS HBA to get all info.
 *
 * This function will query LSI SAS HBA to get all info.
 * @param[out] hba_info HBA Info array.
 * @param[inout] hba_num Number of HBA cards, pass-in the maximum hba number.
 * @param[in] fd File descriptor for redirecting debug message, def. stderr.
 * @retval 0 Success.
 * @retval -1 Failed
 */
int SE_Get_LSI_SAS_HBA_Info(OUT SAS_HBA_INFO *sas_hba,
    INOUT int *hba_num, IN FILE *fd);

/**
 * @fn int SE_Get_Err_Cnt(IN int enc_id, IN int port_id, OUT SAS_HBA_INFO *hba_info,
       OUT int *hba_num, OUT ENCLOSURE_INFO **enc_info, OUT int *num_enc,
       IN FILE *fd)
 * @brief Query specific device's error count.
 *  Hint: This function may allocate memory with
 *      enc_info.exp_ext_info.phy_info[i].att_dev_info.info.resp_raw_u8.data
 *      remember to free it.
 *
 * This function will query specific device's error count by port_id.
 * @param[in] enc_id Enclosure id.
 * @param[in] port_id Physical disk port identifer, -1 for all.
 * @param[OUT] hba_info HBA Info array.
 * @param[OUT] hba_num Number of HBA cards.
 * @param[OUT] enc_info Enc info.
 * @param[in] fd File descriptor for redirecting debug message, def. stderr.
 * @retval 0 Success.
 * @retval -1 Unknow enclosure.
 * @retval -2 Port id not match.
 */
int SE_Get_Err_Cnt(IN int enc_id, IN int port_id, OUT SAS_HBA_INFO *hba_info,
    OUT int *hba_num, OUT ENCLOSURE_INFO **enc_info, OUT int *num_enc,
    IN FILE *fd);

/**
 * @fn int SE_Get_PD_Num(IN int enc_id)
 * @brief Retrieve the number of disks for the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of disks for the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of disks in the specified enclosure.
 */
int SE_Get_PD_Num(IN int enc_id);

/**
 * @fn int SE_Get_PD_Port_Num(IN int enc_id)
 * @brief Retrieve the number of disk ports for the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of disk port for the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of ports in the specified enclosure.
 */
int SE_Get_PD_Port_Num(IN int enc_id);

/**
 * @fn int SE_Get_FAN_Num(IN int enc_id)
 * @brief Retrieve the number of FANs of the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of FANs of the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of FANs in the specified enclosure.
 */
int SE_Get_FAN_Num(IN int enc_id);

/**
 * @fn int SE_Get_TEMP_Num(IN int enc_id)
 * @brief Retrieve the number of TEMPs of the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of TEMPs of the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of TEMPs in the specified enclosure.
 */
int SE_Get_TEMP_Num(IN int enc_id);

/**
 * @fn int SE_Get_Heat_Source_Bitmap(IN int enc_id, IN FAN_REGION region, OUT int *bitmapP)
 * @brief Get the heat source bitmap of specific fan region.
 *
 * This function will get the heat source bitmap of specific fan region.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region The specific fan region.
 * @param[out] bitmapP The heat source bitrmap.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_Heat_Source_Bitmap(IN int enc_id, IN FAN_REGION region, OUT int *bitmapP);

/**
 * @fn int SE_Check_PCI_Blacklist_Exist_In_FAN_Region(IN FAN_REGION region)
 * @brief Check whether the pci device in the blacklist is in the specific fan region.
 *
 * This function will check whether the pci device in the blacklist is in specific fan region.
 * @param[in] region The specific fan region.
 * @retval 1 The pci device in the blacklist is exist in the specific fan region.
 * @retval 0 not exist.
 */
int SE_Check_PCI_Blacklist_Exist_In_FAN_Region(IN FAN_REGION region);

/**
 * @fn int SE_Get_CPU_FAN_Num(IN int enc_id)
 * @brief Retrieve the number of CPU FANs of the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of FANs of the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of FANs in the specified enclosure.
 */
int SE_Get_CPU_FAN_Num(IN int enc_id);

/**
 * @fn int SE_Get_Default_CPU_FAN_Count(IN int enc_id)
 * @brief Retrieve the number of CPU FANs of the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of default CPU FAN count of the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of default CPU FANs in the specified enclosure.
 */
int SE_Get_Default_CPU_FAN_Count(IN int enc_id);

/**
 * @fn int SE_Get_Factory_CPU_FAN_Count(IN int enc_id)
 * @brief Retrieve the number of CPU FANs of the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of maximum H/W capabile CPU FAN count of the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of CPU FANs in the specified enclosure.
 */
int SE_Get_Factory_CPU_FAN_Count(IN int enc_id);

/**
 * @fn int SE_Get_FAN_Region_Num(IN int enc_id)
 * @brief Retrieve the number of FAN control region of the specified enclosure.
 *
 * This function will be invoked to retrieve the number of FAN control region 
 * of specified enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval The number of pre-defined FAN control region. 
 */
int SE_Get_FAN_Region_Num(IN int enc_id);

/**
 * @fn int SE_Get_Temp_Region_Num(IN int enc_id)
 * @brief Retrieve the number of region temperature of the specified enclosure.
 *
 * This function will be invoked to retrieve the number of region temperature defined
 * of specified enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval The number of pre-defined region temperature.
 */
int SE_Get_Temp_Region_Num(IN int enc_id);

/**
 * @fn int SE_Get_Extran_FAN_Region_Num(IN int enc_id)
 * @brief Retrieve the number of FAN control regions of specified enclosure, exclude System FAN & CPU FAN.
 *
 * This function will be invoked to retrieve the number of FAN control regions of specified enclosure
 * , exclude System FAN & CPU FAN of the specified enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval The number of of FAN control regions of specified enclosure exclude System FAN & CPU FAN of the specified enclosure.
 */
int SE_Get_Extran_FAN_Region_Num(IN int enc_id);

/**
 * @fn int SE_Get_FAN_Region_FAN_Count(IN int enc_id, IN int region);
 * @brief Retrieve the number of FANs of the specified region of a specified enclosure.
 *
 * This function will be invoked to retrieve the number of FANs of the specified region of a specified enclosure
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region Region identifier in the enclosure (0 based).
 * @retval FANs of the specified region of a specified enclosure.
 */
int SE_Get_FAN_Region_FAN_Count(IN int enc_id, IN int region);

/**
 * @fn int SE_Get_FAN_Region_Name(IN int enc_id, IN int region, OUT char* bufP, IN int buf_len);
 * @brief Retrieve the alias name and the fan id in the REGION_TYPE enumeration of the specified fan region
 *
 * This function will be invoked to get the alias name and the fan id in the REGION_TYPE enumeration of the specified fan region
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_id Region identifier in the enclosure (1 based).
 * @param[out] bufP buffer pointer to store returned fan region name.
 * @param[in] buf_len length of the input buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_FAN_Region_Name(IN int enc_id, IN int region_id, OUT char* bufP, IN int buf_len);

/**
 * @fn int SE_Get_Temp_Region_Name(IN int enc_id, IN int region, OUT char* bufP, IN int buf_len);
 * @brief Retrieve the alias name and the temp. id in the REGION_TYPE enumeration of the specified temp. region
 *
 * This function will be invoked to get the alias name and the temp. id in the REGION_TYPE enumeration of the specified temp. region
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_id Region identifier in the enclosure (1 based).
 * @param[out] bufP buffer pointer to store returned temp. region name.
 * @param[in] buf_len length of the input buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_Temp_Region_Name(IN int enc_id, IN int region_id, OUT char* bufP, IN int buf_len);

/**
 * @fn int SE_Get_FAN_Region_Type(IN int enc_id, IN int region, OUT REGION_TYPE *type_idP);
 * @brief Retrieve the fan id in the REGION_TYPE enumeration of the specified fan region
 *
 * This function will be invoked to get the fan id in the REGION_TYPE enumeration of the specified fan region
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_id Region identifier in the enclosure (1 based).
 * @param[out] type_idP the fan id in the REGION_TYPE enumeration.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_FAN_Region_Type(IN int enc_id, IN int region_id, OUT REGION_TYPE *type_idP);

/**
 * @fn int SE_Get_Temp_Region_Type(IN int enc_id, IN int region, OUT REGION_TYPE *type_idP);
 * @brief Retrieve the temp. id in the REGION_TYPE enumeration of the specified temp. region
 *
 * This function will be invoked to get the temp. id in the REGION_TYPE enumeration of the specified temp. region
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_id Region identifier in the enclosure (1 based).
 * @param[out] type_idP the temp. id in the REGION_TYPE enumeration.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_Temp_Region_Type(IN int enc_id, IN int region_id, OUT REGION_TYPE *type_idP);

/**
 * @fn void SE_Get_FAN_Region_Enum(IN int enc_id, OUT FAN_REGION *ary, OUT int *ary_count);
 * @brief Enumerate all the fan regions(include the Sys/CPU fan) in the system
 *
 * This function enumerates all the fan regions(include the Sys/CPU fan) in the system
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] ary Buffer pointer to store returned fan region renueration
 * @param[out] ary_count Number of fan regions enuerated.
 */
void SE_Get_FAN_Region_Enum(IN int enc_id, OUT FAN_REGION *ary, OUT int *ary_count);

/**
 * @fn void SE_Get_Temp_Region_Enum(IN int enc_id, OUT TEMP_REGION *ary, OUT int *ary_count);
 * @brief Enumerate all the temp. regions(exclude the SYS/ CPU temp, only the special region defied) in the system
 *
 * This function enumerates all the temp. regions(exclude the SYS/ CPU temp, only the special region defied) in the system
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] ary Buffer pointer to store returned temp. region renueration
 * @param[out] ary_count Number of temp. regions enuerated.
 */
void SE_Get_Temp_Region_Enum(IN int enc_id, OUT TEMP_REGION *ary, OUT int *ary_count);

/**
 * @fn int SE_Is_TFAN_Support(IN int enc_id, IN FAN_REGION region_idx)
 * @brief If the specified fan region runs TFAN algorithm
 *
 * This function return the result if the specified fan region runs TFAN algorithm
 * of specified enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_idx FAN control region index.
 * @retval If the specified fan region runs TFAN algorithm.
 */
int SE_Is_TFAN_Support(IN int enc_id, IN FAN_REGION region_idx);

/**
 * @fn int SE_Get_FAN_Region_Scrollbar_Support(OUT int* fan_region)
 * @brief Check if the QTS uses scroll bar in the fixed fan mode option
 *
 * This function is called to inquiry if the QTS uses scroll bar in the fixed fan mode option
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 1 Scroll bar is supportted.
 * @retval 0 Scroll bar is not supported.
 */
int SE_Get_FAN_Region_Scrollbar_Support(IN int enc_id, IN FAN_REGION region_idx);

/**
 * @fn int SE_Get_FAN_Region_TFan_Optional_Support(OUT int* fan_region)
 * @brief Check if the TFan mode is supported
 *
 * This function is called to inquiry if the TFan mode is supported
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 1 if the TFan mode is supported.
 * @retval 0 if the TFan mode is not supported
 */
int SE_Get_FAN_Region_TFan_Optional_Support(IN int enc_id, IN FAN_REGION region_idx);

/**
  @fn SE_Get_FAN_Region_Stepping(IN int enc_id, IN int region_idx, OUT int steppingP)
 * @brief Retrieve the stepping setting of specified FAN control region.
 *
 * This function will be invoked to retrieve the current stepping setting
 * of specified FAN control region from persistent configuration file.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_idx FAN control region index.
 * @param[out] steppingP A pointer to store stepping setting.
 * @retval 1-100 The current stepping setting.
 * @retval Others Generic failure.
 */
int SE_Get_FAN_Region_Stepping(IN int enc_id, IN FAN_REGION region_idx, OUT int* stepping);

/**
 * @fn SE_Set_FAN_Region_Stepping(IN int enc_id, IN int region_idx, IN int stepping)
 * @brief Change the stepping setting of specified FAN control region.
 *
 * This function will store the stepping setting to persistent configuration file,
 * calculate the corresponding PWM based on stepping value, then control ALL fans in
 * specified FAN control region by invoking HM_Set_Fan_Speed_By_PWM() function.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_idx FAN control region index.
 * @param[out] stepping User provide stepping value (1-100).
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Set_FAN_Region_Stepping(IN int enc_id, IN FAN_REGION region_idx, IN int stepping);

/**
 * @fn int SE_Get_FAN_Disk_Bitmap(IN int enc_id, IN FAN_REGION region_idx)
 * @brief Retrieve the disk bitmap of specified enclosure for different fan region.
 *
 * This function will be invoked to get disk bitmap of specified enclsoare
 * for different fan region.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_idx FAN control region index.
 * @retval disk bitmap or -1 for all disks.
 */
int SE_Get_FAN_Disk_Bitmap(IN int enc_id, IN FAN_REGION region_idx);

/**
 * @fn int SE_Get_FAN_Cache_Disk_Bitmap(IN FAN_REGION region_idx, OUT int* bitmap)
 * @brief Retrieve the disk bitmap of cache enclosure for different fan region.
 *
 * This function will be invoked to get disk bitmap of cache enclosuare
 * for different fan region.
 * @param[in] region_idx FAN control region index.
 * @param[out] bitmap disk bitmap.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_FAN_Cache_Disk_Bitmap(IN FAN_REGION region_idx, OUT int* bitmap);

/**
 * @fn int SE_Get_FAN_PCIE_Slot_Bitmap(IN FAN_REGION region_idx, OUT int* bitmap)
 * @brief Retrieve the pcie slot bitmap of pcie enclosure for different fan region.
 *
 * This function will be invoked to get pcie slot bitmap of pcie enclosuare
 * for different fan region.
 * @param[in] region_idx FAN control region index.
 * @param[out] bitmap pcie slot bitmap.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_FAN_PCIE_Slot_Bitmap(IN FAN_REGION region_idx, OUT int* bitmap);

/**
 * @fn int SE_Get_Id_By_Sys_Id(IN char* enc_sys_id, OUT int* enc_idP)
 * @brief Retrieve the logical enclosure identifier via system dependent identifier.
 *
 * This function is called by PnP utility to obtain the assigned logical identifier from the
 * internal configuration file.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[out] enc_idP a logical enclosure identifier that has been assigned to this enclosure.
 * @retval 0 Success, the specified enclosure identifier is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified enclosure identifier in the
 *         internal configurationturn file.
 */
int SE_Get_Id_By_Sys_Id(IN char* enc_sys_id, OUT int* enc_idP);

/**
 * @fn int SE_Scan(IN int scan_all)
 * @brief Scan the connected storage enclosure and store them in the internal
 *        configuration file.
 *
 * This function will remove the original enclosure setting in the internal
 * configuration file and invoke the system dependent enumerate function to re-generate them.
 * @param[in] scan_all Scan root enclosure only when it equal to zero, otherwise scan all enclosure.
 * @retval Return the count of the available storage enclosures.
 */
int SE_Scan(IN int scan_all);

/**
 * @fn int SE_Set_Buzzer(IN int enc_id, IN BUZZER_MODE mode)
 * @brief Set the enclosure buzzer in the specified mode.
 *
 * @param[in] enc_id Enclosure identifier.
 * @param[in] mode SHORT period (0) or LONG period (1).
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Set_Buzzer(IN int enc_id, IN BUZZER_MODE mode);

/**
 * @fn int SE_Enable_Buzzer(IN int enc_id, IN int enable)
 * @brief Enable/ Disable the buzzer function of the specified enclosure.
 *
 * @param[in] enc_id Enclosure identifier.
 * @param[in] enable Enable or disable the buzzer function of the specified enclosure.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Enable_Buzzer(IN int enc_id, IN int enable);

/**
 * @fn int SE_Set_Sound_Alert(IN int enc_id, IN VOICE_ID voice_id)
 * @brief Set the enclosure buzzer in the specified mode.
 *
 * @param[in] enc_id Enclosure identifier.
 * @param[in] voice_id ID of the voice.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Set_Sound_Alert(IN int enc_id, IN VOICE_ID voice_id);

/**
 * @fn int SE_Is_Internal(IN int enc_id)
 * @brief Check if the specified enclosure is internal.
 *
 * The physical disks in the internal enclosure could be created
 * as a RAID volume.
 * @param[in] enc_id Enclosure identifier.
 * @retval 1 TRUE, the specified enclosure is internal.
 * @retval 0 FALSE, the specified enclosure is not internal.
 */
int SE_Is_Internal(IN int enc_id);

/**
 * @fn int SE_Is_Exist(IN int enc_id)
 * @brief Check if the specified enclosure exists.
 *
 * Check if the specified enclosure is connected or not.
 * @param[in] enc_id Enclosure identifier.
 * @retval 1 TRUE, the specified enclosure exists.
 * @retval 0 FALSE, the specified enclosure does not exist.
 */
int SE_Is_Exist(IN int enc_id);

/**
 * @fn int SE_Is_Removing(IN int enc_id)
 * @brief Check if the specified enclosure is marked as removing.
 *
 * Check if the specified enclosure is removing or not.
 * @param[in] enc_id Enclosure identifier.
 * @retval 1 TRUE, the specified enclosure is removing.
 * @retval 0 FALSE, the specified enclosure is not removing.
 */
int SE_Is_Removing(IN int enc_id);

/**
 * @fn int SE_Is_Relinking(IN int enc_id)
 * @brief Check if the specified enclosure is marked as relinking.
 *
 * Check if the specified enclosure is relinking or not.
 * @param[in] enc_id Enclosure identifier.
 * @retval 1 TRUE, the specified enclosure is relinking.
 * @retval 0 FALSE, the specified enclosure is not relinking.
 */
int SE_Is_Relinking(IN int enc_id);

/**
 * @fn int SE_Send_Heartbeat(IN int enc_id)
 * @brief Send the hearbeat signal to the specified enclosure.
 *
 * The NAS will send the heartbeat signal to the enclosure every 3 mins.
 * @param[in] enc_id Enclosure identifier.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Send_Heartbeat(IN int enc_id);

/**
 * @fn int SE_Get_Friendly_Name(IN int enc_id, OUT char* friendly_name, IN int buf_len)
 * @brief Retrieve the nickname of the specified enclosure.
 *
 * This function is called by system utility to obtain the nickname of specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[out] friendly_name Enclosure nickname.
 * @param[in] buf_len The buffer count for the friendly_name.
 * @retval 0 Success, the specified nickname is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the nickname of the specified enclosure.
 */
int SE_Get_Friendly_Name(IN int enc_id, OUT char* friendly_name, IN int buf_len);

/**
 * @fn int SE_Set_Friendly_Name(IN int enc_id, IN char* friendly_name)
 * @brief Set up the nickname for the specified enclosure.
 *
 * This function is called by system utility to write down the nickname of specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] friendly_name Enclosure nickname.
 * @retval 0 Success, the specified nickname is written down completely.
 * @retval -1 Generic failure, Fail to write down the nickname of the specified enclosure.
 */
int SE_Set_Friendly_Name(IN int enc_id, IN char* friendly_name);

/**
 * @fn int SE_Get_Serial_No(IN int enc_id, OUT char* serial_no, IN unsigned int buf_len)
 * @brief Retrieve the serial number for the specified storage enclosure.
 *
 * This function is called by system utility to obtain the serial number for specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[out] serial_no Enclosure serial number.
 * @param[in] buf_len The buffer count for the serial_no.
 * @retval 0 Success, the specified serial number is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the serial number of the enclosure in the
 *         internal configurationturn file.
 */
int SE_Get_Serial_No(IN int enc_id, OUT char* serial_no, IN unsigned int buf_len);

/**
 * @fn int SE_Set_Serial_No(IN int enc_id, IN char* serial_no)
 * @brief Set up the serial number for the specified enclosure.
 *
 * This function is called by system utility to write down the serial number of specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] serial_no Enclosure serial number.
 * @retval 0 Success, the specified serial number is written down completely.
 * @retval -1 Generic failure, Fail to write down the serial number of the specified enclosure.
 */
int SE_Set_Serial_No(IN int enc_id, IN char* serial_no);

/**
 * @fn int SE_Get_Model(IN int enc_id, OUT char* model, IN unsigned int buf_len)
 * @brief Retrieve the model name for the specified storage enclosure.
 *
 * This function is called by system utility to obtain the model name for specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[out] model Enclosure model name.
 * @param[in] buf_len The buffer count for the model name.
 * @retval 0 Success, the specified model name is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the model name of the enclosure in the
 *         internal configurationturn file.
 */
int SE_Get_Model(IN int enc_id, OUT char* model, IN unsigned int buf_len);

/**
 * @fn int SE_Get_LCM_Dev_Name(IN int enc_id, OUT char* dev_name, IN int buf_len)
 * @brief Get LCM device name for the specified enclosure.
 *
 * This function is called by system utility to get LCM device name  of specified enclosure.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[out] dev_name LCM device name.
 * @param[in] buf_len The buffer count for the device name.
 * @retval 0 Success, the specified LCM device name is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the LCM device name of the specified enclosure.
 */
int SE_Get_LCM_Dev_Name(IN int enc_id, OUT char* dev_name, IN int buf_len);

/**
 * @fn int SE_Get_Connector_Status(IN int enc_id, OUT int* status_ary, IN int status_ary_len)
 * @brief Get each connector status of SAS JBOD
 *
 * This function will get each connector status of SAS JBOD
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[out] status_ary JBOD connector status array, use PHY_LINK_LOSS or PHY_LINK_MISCONNECT to check the status.
 * @param[in] status_ary_len JBOD connector status array length.
 * @retval Return the count of the available connector status, if the count is larger than
 *         status_ary_len, the calling utility should allocate more space for identifier array to
 *         obtain all available enclosure connector status.
 */
int SE_Get_Connector_Status(IN int enc_id, OUT int* status_ary, IN int status_ary_len);

/**
 * @fn int SE_Get_System_Status(IN int enc_id, OUT ENCLOSURE_SYS_STATUS *enc_sys_statusP)
 * @brief Get Enclosure system status from file.
 *
 * This function is called to get all available enclosure status(e.g temperature, fan status,
 * and power status), these values are written by hal_util.
 * @param[in] enc_id Enclosure identifier.
 * @param[out] enc_sys_statusP Enclosure system status pointer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Get_System_Status(IN int enc_id, OUT ENCLOSURE_SYS_STATUS *enc_sys_statusP);

/**
 * @fn int SE_Upgrade_FW(IN int enc_id, IN char* img_path, OUT char* new_versionP, IN int buf_len)
 *
 * @brief Storage enclosure firmware upgrade exclude the root enclosure (NAS itself).
 *
 * Only support SAS enclosure right now.
 * @param[in] enc_id logical enclosure identifier.
 * @param[in] img_path The single upgrade image file path.
 * @param[out] new_versionP The image version after upgarding successfully.
 * @param[in] buf_len The buffer ocunt for new_versionP.
 * @retval 0 Success, The specified enclosure has been upgraded to the specified binary image.
 * @retval < 0 Generic failure, Fail to update the specified binary image.
 */
int SE_Upgrade_FW(IN int enc_id, IN char* img_path, OUT char* new_versionP, IN int buf_len);

/**
 * @fn int SE_Reboot(IN int enc_id)
 *
 * @brief Reboot the specified storage enclosure.
 *
 * Only support SAS enclosure right now.
 * @param[in] enc_id logical enclosure identifier, use ALL_ENCLOSURES for all enclousres.
 * @retval > 0 Success, The number of enclosures which has been rebooted successfully.
 * @retval < 0 Generic failure, Fail to reboot the specified enclosure.
 */
int SE_Reboot(IN int enc_id);

/**
 * @fn int SE_Upgrade_SR_FW(IN int enc_id, IN int port_id, IN char* img_path, OUT char* new_versionP, IN int buf_len)
 *
 * @brief Storage enclosure SR firmware upgrade.
 *
 * @param[in] enc_id logical enclosure identifier.
 * @param[in] img_path The single upgrade image file path.
 * @param[out] new_versionP The image version after upgarding successfully.
 * @param[in] buf_len The buffer ocunt for new_versionP.
 * @retval 0 Success, The specified enclosure has been upgraded to the specified binary image.
 * @retval < 0 Generic failure, Fail to update the specified binary image.
 */
int SE_Upgrade_SR_FW(IN int enc_id, IN int port_id, IN char* img_path, OUT char* new_versionP, IN int buf_len);

/**
 * @fn int SE_Hotswap_Check_PD(int enc_id, int port_id)
 * @brief Check specified pd exist in se hotswap file.
 *
 * This function is called when pd attached.
 * It will check the specified pd in hotswap file.
 * If the answer is yes, it means the PD is exist when
 * enclosure removed. It also reset the pd_bitmap
 * in hotswap file.
 * @param enc_id Enclosure identifier.
 * @param port_id Port identifier.
 * @retval 1 True.
 * @retval 0 False.
 */
int SE_Hotswap_Check_PD(int enc_id, int port_id);

/**
 * @fn int SE_Hotswap_Clear_Enc(int enc_id)
 * @brief Clear specified enclosure in se hotswap file.
 *
 * This function is called when recover storage.
 * It will clear enclosure section in hotswap file.
 * @param enc_id Enclosure identifier.
 * @retval 0 Success, section has been removed completely.
 * @retval -1 Generic failure.
 */
int SE_Hotswap_Clear_Enc(int enc_id);

/**
 * @fn int SE_Set_REXP_Phys_Err_Handling_Of_Ports(IN int enc_id, IN int enable,
 *     IN int def, IN uint8_t err_thr, IN uint8_t rst_cnt, IN uint8_t poll_sec)
 * @brief Control phy error counts handling behavior of REXP.
 *
 * This function will set the parameters to control error counts monitor
 *      behavior of REXP.
 * @param[in] enc_id Enclosure ID.
 * @param[in] enable Whether enable(1) error counts monitor.
 * @param[in] def Whether use default setting(100,3,1).
 * @param[in] err_thr Error counts threshold(def. 100).
 * @param[in] rst_cnt If over threshold need to reset which times(def. 3).
 * @param[in] poll_sec Polling time (unit:sec, def:1).
 * @retval 0 Success.
 * @retval Otherwise, return by system().
 */
int SE_Set_REXP_Phys_Err_Handling_Of_Ports(IN int enc_id, IN int enable,
    IN int def, IN uint8_t err_thr, IN uint8_t rst_cnt, IN uint8_t poll_sec);

/**
 * @fn int SE_Get_REXP_Enc_Id(OUT int *bitmapP)
 * @brief Retrieve the enclosure identifier of the REXP disk enclosure.
 *
 * This function is called by nas utility to obtain the REXP enclosure identifier bitmap which
 * could be used to find the internal REXP disk later.
 * @param bitmapP[in] Pointer of port bitmap for interal enclosures which support expansion.
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Get_REXP_Enc_Id(OUT int *bitmapP);

/**
 * @fn int SE_Get_VJBOD_Enc_Id(OUT int *bitmapP)
 * @brief Retrieve the enclosure identifier of the Virtual JBOD disk enclosure.
 *
 * This function is called by nas utility to obtain the Virtual JBOD enclosure identifier bitmap which
 * could be used to find the internal Virtual JBOD disk later.
 * @param bitmapP[in] Pointer of port bitmap for interal enclosures which support expansion.
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Get_VJBOD_Enc_Id(OUT int *bitmapP);

/**
 * @fn int SE_Get_Enc_Id_by_Protocol(OUT int *bitmapP, IN ENCLOSURE_PROTOCOL protocol_type)
 * @brief Retrieve the identifier bitmap of enlsoures with the specified protocol_type.
 *
 * This function is called by nas utility to obtain the identifier bitmap of enlsoures with the specified protocol_type.
 * @param bitmapP[out] bitmapP, Pointer of a bitmap for interal enclosures which support expansion.
 * @param bitmapP[in] protocol_type, A specified protocol_type.
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Get_Enc_Id_by_Protocol(OUT int *bitmapP, IN ENCLOSURE_PROTOCOL protocol_type);

/**
 * @fn int SE_Get_Cache_Enc_Id(void)
 * @brief Retrieve the enclosure identifier of the internal SSD cache enclosure.
 *
 * This function is called by nas utility to obtain the internal cache enclosure identifier which
 * could be used to find the internal SSD cache later.
 * @retval > 0 Success, the internal cache enclosure identifier.
 * @retval Otherwise, generic failure, fail to retrieve the cache enclosure identifier.
 */
int SE_Get_Cache_Enc_Id(void);

/**
 * @fn int SE_Get_ODD_Enc_Id(void)
 * @brief Retrieve the enclosure identifier of the ODD enclosure.
 *
 * This function is called by nas utility to obtain the the enclosure identifier of the ODD enclosure.
 * @retval > 0 Success, the internal cache enclosure identifier.
 * @retval Otherwise, generic failure, fail to retrieve the cache enclosure identifier.
 */
int SE_Get_ODD_Enc_Id(void);

/**
 * @fn int SE_Get_Pci_Cache_Enc_Id(OUT int *bitmapP)
 * @brief Retrieve the enclosure identifier of the PCI cache enclosure.
 *
 * This function is called by nas utility to obtain the PCI cache enclosure identifier bitmap which
 * could be used to find the internal PCI cache later.
 * @param[out] bitmapP Pointer of port bitmap for interal enclosures which support PCI cache
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Get_Pci_Cache_Enc_Id(OUT int *bitmapP);

/**
 * @fn int SE_Get_MMC_Enc_Id(void)
 * @brief Retrieve the enclosure identifier of the MMC enclosure.
 *
 * This function is called by nas utility to obtain the MMC enclosure identifier.
 * @retval > 0 Success, the MMC enclosure identifier.
 * @retval Otherwise, generic failure, fail to retrieve the MMC identifier.
 */
int SE_Get_MMC_Enc_Id(void);

/**
 * @fn int SE_Get_Cacheable_Port_Bitmap(IN int enc_id, OUT int* bitmap)
 * @brief Retrieve the port bitmap which support SSD cache for particular enclosure
 *
 * This function is called by nas utility to obtain the port bitmap which can be used as SSD cache.
 * @param[in] enc_id Enclosure identifier.
 * @param[out] bitmap Port bitmap which support SSD cache for specified enclosure.
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Get_Cacheable_Port_Bitmap(IN int enc_id, OUT int* bitmap);

/**
 * @fn int SE_Is_USB_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is the USB JBOD.
 *
 * This function is called by nas utility to check the specified JBOD type.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a USB JBOD.
 * @retval 0 FALSE, the specified enclosure is not a USB JBOD.
 */
int SE_Is_USB_JBOD(IN int enc_id);

/**
 * @fn int SE_Is_TR_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is the TR JBOD.
 *
 * This function is called by nas utility to check if the specified JBOD is a TR one.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a TR JBOD.
 * @retval 0 FALSE, the specified enclosure is not a TR JBOD.
 */
int SE_Is_TR_JBOD(IN int enc_id);

/**
 * @fn int SE_Is_TB_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is the Thunderbolt JBOD.
 *
 * This function is called by nas utility to check the specified JBOD type.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a Thunderbolt JBOD.
 * @retval 0 FALSE, the specified enclosure is not a Thunderbolt JBOD.
 */
int SE_Is_TB_JBOD(IN int enc_id);

/**
 * @fn int SE_Is_QM2_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is the QM2 JBOD.
 *
 * This function is called to check the specified JBOD type.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a QM2 JBOD.
 * @retval 0 FALSE, the specified enclosure is not a QM2 JBOD.
 */
int SE_Is_QM2_JBOD(IN int enc_id);

/**
 * @fn int SE_Sys_Do_Notify_Log(IN char *enc_sys_id, IN int parent_port_id, IN int remove_record)
 * @brief add a notify log for particular enclosure
 *
 * This function is called by nas utility to add a notify log for particular enclosure.
 * @param[in] enc_sys_id enclosure id.
 * @param[in] parent_port_id parent port id.
 * @param[in] remove_record remove record.
 * @retval 0 Success.
 * @retval Otherwise, generic failure.
 */
int SE_Sys_Do_Notify_Log(IN char *enc_sys_id, IN int parent_port_id, IN int remove_record);

/**
 * @fn int SE_Is_TL_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is the TL JBOD.
 *
 * This function is called by nas utility to check if the specified JBOD is a TL one.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a TL JBOD.
 * @retval 0 FALSE, the specified enclosure is not a TL JBOD.
 */
int SE_Is_TL_JBOD(IN int enc_id);

/**
 * @fn int TL_Get_FW_VER(IN int enc_id, OUT char *tl_ver, IN int tl_ver_len)
 * @brief Check if the specified enclosure is the TL JBOD.
 *
 * This function is called by nas utility to get the TL Pic version.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] tl_ver TL Pic Version.
 * @param[in] tl_ver_len Length of TL Pic Version.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TL_Get_FW_VER(IN int enc_id, OUT char *tl_ver, IN int tl_ver_len);

/**
 * @fn int TL_Set_System_FAN(IN int enc_id, IN int level)
 * @brief Set TL JBOD system FAN speed.
 *
 * This function is called by nas utility to set TL JBOD system FAN speed.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] level: Auto/High/Medium/Low
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TL_Set_System_FAN(IN int enc_id, IN int level);

/**
 * @fn int TL_Set_Warning_Temp(IN int enc_id, IN int temperature)
 * @brief Set warning temperature of TL model.
 *
 * This function is called by nas utility to set warning temperature of TL model.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] temperature: Temperatue is Celsius
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TL_Set_Warning_Temp(IN int enc_id, IN int temperature);

/**
 * @fn int TL_Get_Switch_Mode(IN int enc_id, OUT int *mode)
 * @brief Get switch mode of TL model.
 *
 * This function is called by nas utility to get switch mode of TL model.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] mode: Provide hardware switch mode and definition
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TL_Get_Switch_Mode(IN int enc_id, OUT int *mode);

/**
 * @fn int SE_Is_External_JBOD(IN int enc_id)
 * @brief Check if the specified enclosure is external JBOD(TR/TL).
 *
 * This function is called by nas utility to check the specified JBOD type.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the specified enclosure is a external JBOD.
 * @retval 0 FALSE, the specified enclosure is not a external JBOD.
 */
int SE_Is_External_JBOD(IN int enc_id);

/**
 * @fn int Ext_Port_Scan(IN char *enc_sys_id, OUT EXT_PORT_INFO *ext_port_info)
 * @brief Scan all external ports for the specified enc_sys_id
 *
 * This function will scan all external ports for the specified enc_sys_id
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[out] ext_port_info the external minisas port info. structure.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int Ext_Port_Scan(IN char *enc_sys_id, OUT EXT_PORT_INFO *ext_port_info);

/**
 * @fn int SE_Get_QXP_Ext_Port_Info(IN int enc_id, OUT EXT_PORT_INFO *ext_port_info);
 * @brief Retrieve the external port info of the specified enc_id.
 *
 * This function is called to retrieve the external port info of the specified enc_id
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] ext_port_info the external minisas port info. structure.
 * @retval 0 Success, retrieve the external minisas port info. successfully.
 * @retval -1 Fail to retrieve the external minisas port info.
 */
int SE_Get_QXP_Ext_Port_Info(IN int enc_id, OUT EXT_PORT_INFO *ext_port_info);

/**
 * @fn int SE_Is_Port_Attach_JBOD(IN int tbt_port)
 * @brief Check if the specified tbt port connected with Thunderbolt JBOD or QNA device.
 *
 * This function is called by utilities to check if the specified tbt port connected with Thunderbolt JBOD or QNA device.
 * @param[in] tbt_port thunderbolt JBOD identifier (0 based).
 * @retval 2 TRUE, the specified port is connecting with a QNA device.
 * @retval 1 TRUE, the specified port is connecting with a Thunderbolt JBOD.
 * @retval 0 FALSE, the specified port is not connecting with a Thunderbolt JBOD.
 */
int SE_Is_Port_Attach_JBOD_QNA(IN int tbt_port);

/**
 * @fn int SE_Is_Disk_Data_Collection_Enabled(void)
 * @brief Check if the function of disk data collection was enabled.
 *
 * This function is called by utilities to check if the function of disk data collection was enabled.
 * @retval 1 Disk data collection is supportted.
 * @retval 0 Disk data collection is not supported.
 */
int SE_Is_Disk_Data_Collection_Enabled(void);

/**
 * @fn int SE_Attached_TBT_JBOD_Count(IN int tbt_port)
 * @brief Check if the specified tbt port connected with Thunderbolt JBOD.
 *
 * This function is called by utilities to check if the specified tbt port connected with Thunderbolt JBOD.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval number of tbt jbods that attach to the specified tbt_port.
 */
int SE_Attached_TBT_JBOD_Count(IN int tbt_port);

// SWChen enhence event log 20131122
/**
 * @fn int SE_Get_Log_Name(IN int enc_id, OUT char* buf, IN int buf_len)
 * @brief Get the enclosure name for system event log
 *
 * This function is used when output disk name in system event log
 * @param[in] enc_id Enclosure identifier.
 * @param[out] buf Buffer to store the enclosure name.
 * @param[in] buf_len Buffer length.
 * @retval 0 Get Log name successfully.
 * @retval 1 Get Slot Log name successfully, 
 * @retval -1 General error. 
 */
int SE_Get_Log_Name(IN int enc_id, OUT char *buf, IN int buf_len);

/**
 * @fn int SE_Get_Enc_Type(IN int enc_id, OUT ENC_TYPE *type)
 * @brief Get the enclosure type for resource monitor
 *
 * This function is called by cgi to obtain the enclosure type
 * @param[in] enc_id Enclosure identifier.
 * @param[out] type to store the enclosure type.
 * @retval 0 Get enclosure type successfully.
 * @retval -1 General error.
 */
int SE_Get_Enc_Type(IN int enc_id, OUT ENC_TYPE *type);

/**
 * @fn int SE_Get_LED_BV_Ctrl_Capability(IN int enc_id, OUT LED_BV_CTRL *cap)
 * @brief Get led brightness control capability.
 *
 * This function is used to get the led brightness control capability.
 * @param[in] enc_id Enclosure identifier.
 * @param[out] cap Capability to control led brightness value.
 * @retval 0 Success.
 * @retval -1 Otherwise, generic failure.
 */
int SE_Get_LED_BV_Ctrl_Capability(IN int enc_id, OUT LED_BV_CTRL *cap);

/**
 * @fn int SE_Get_LED_Scrollbar_Support(IN int enc_id)
 * @brief Check if the QTS could control the brightness of the led by using a scrollbar
 *
 * This function is called to inquiry if the QTS could control the brightness of the led by using a scrollbar
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 Scroll bar is supportted.
 * @retval 0 Scroll bar is not supported.
 */
int SE_Get_LED_Scrollbar_Support(IN int enc_id);

/**
 * @fn int SE_Get_SAS_All_Ports_Linkrate(IN char *enc_sys_id, OUT int *link_rate, IN int buf_len, OUT int *port_num)
 * @brief Get the enclosure name for system event log
 *
 * This function is used when output disk name in system event log
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[out] link_rate Buffer to store the linkrate of each port.
 * @param[in] buf_len Buffer length.
 * @param[out] port_num to store the number of the ports.
 * @retval 0 Success.
 * @retval -1 Otherwise, generic failure.
 */
int SE_Get_SAS_All_Ports_Linkrate(IN char *enc_sys_id, OUT int *link_rate, IN int buf_len, OUT int *port_num);

/**
 * @fn int SE_Get_SAS_Address(IN char *enc_sys_id, OUT uint64_t *sas_addr)
 * @brief Get the enclosure's sas address.
 *
 * This function will get the sas address of specific enclosure.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[out] sas addr SAS address.
 * @retval 0 Success.
 * @retval -1 Otherwise, generic failure.
 */
int SE_Get_SAS_Address(IN char *enc_sys_id, OUT uint64_t *sas_addr);

/**
 * @fn int SE_Support_Locate_Led(IN char *enc_sys_id)
 * @brief Check if the specified enclosure could be located.
 *
 * This function is called by nas utility to check the specified enclosure could be located.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @retval 1 TRUE, the led of the specified enclosure can be located.
 * @retval 0 FALSE, the led of the specified enclosure can not be located.
 */
int SE_Support_Locate_Led(IN char *enc_sys_id);

/**
 * @fn int SE_Get_PD_Env_Sensor_Num(IN int enc_id)
 * @brief Retrieve the number of PD environment sensors for the specified enclosure.
 *
 * This function is called by nas utility to obtain the number of PD environment sensors for the specified enclosure.
 * @param[in] enc_id logical enclosure identifier.
 * @retval The number of PD environment sensors in the specified enclosure.
 */
int SE_Get_PD_Env_Sensor_Num(IN int enc_id);

/**
 * @fn int SE_Get_Disk_Power_Status(IN int enc_id, OUT SE_DISK_POWER_STATUS *statusP)
 * @brief Retrieve all of physical disk power status.
 *
 * This function is called to obtain the power status of the specified enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] statusP The power status of the specified enclosure.
 * @retval 0 Success.
 * @retval -1 Generic failure (include SE haven't any disk).
 */
int SE_Get_Disk_Power_Status(IN int enc_id, OUT SE_DISK_POWER_STATUS *statusP);

/**
 * @fn void SE_Remove_and_Backup_Conf(IN int enc_id)
 * @brief Backup and remove the specified SE config file
 *
 * This function is used to backup and remove the specified SE config file
 * @param[in] enc_id Enclosure identifier.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Remove_and_Backup_Conf(IN int enc_id);

/**
 * @fn int SE_Check_MCU_Support(IN int enc_id)
 * @brief Check whether the MCU is support in specific enclosure.
 *
 * This function will check whether the MCU is support in specific enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 MCU supported.
 * @retval 0 MCU not supported.
 */
int SE_Check_MCU_Support(IN int enc_id);

/**
 * @fn int SE_Check_MCU_Is_Spec3(IN int enc_id)
 * @brief Check whether the MCU command spec is spec3.
 *
 * This function will check whether the MCU command spec is spec3.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 MCU Spec3 supported.
 * @retval 0 MCU is not Spec3.
 */
int SE_Check_MCU_Is_Spec3(IN int enc_id);

/**
 * @fn int SE_Check_MCU_Is_Spec4(IN int enc_id)
 * @brief Check whether the MCU command spec is spec3.
 *
 * This function will check whether the MCU command spec is spec3.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 MCU Spec3 supported.
 * @retval 0 MCU is not Spec3.
 */
int SE_Check_MCU_Is_Spec4(IN int enc_id);

/**
 * @fn int SE_Get_MCU_Version(IN int enc_id, OUT char *version, IN int buf_len)
 * @brief Get the MCU version in specific enclosure.
 *
 * This function will Get the MCU version and build date in specific enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] version ${MCU_version}-${MCU_build_date}.
 * @param[in] buf_len The length of buffer.
 * @retval 0 Success.
 * @retval else Failed.
 */
int SE_Get_MCU_Version(IN int enc_id, OUT char *version, IN int buf_len);

/**
 * @fn int SE_Check_MCU_Need_Update(IN int enc_id, OUT char *cur_ver, IN int cur_len, OUT char *exp_ver, IN int exp_len, OUT int *needed)
 * @brief Check the MCU version whether need to update.
 *
 * This function will check the MCU version whether need to update.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] cur_ver The MCU current version without build date.
 * @param[in] cur_len The length of MCU current version without build date.
 * @param[out] exp_ver The MCU least version without build date.
 * @param[in] exp_len The length of MCU least version without build date.
 * @param[out] needed 1: Need to update, 0: Need not update.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int SE_Check_MCU_Need_Update(IN int enc_id, OUT char *cur_ver, IN int cur_len, OUT char *exp_ver, IN int exp_len, OUT int *needed);

/**
 * @fn SE_Ini_MCU_FAN_Num(IN int enc_id, INOUT int *fan_num)
 * @brief Initial FAN number to MCU.
 *
 * This function will initail fan number to MCU
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[inout] fan_num FAN number.
 * @retval >= 0 Success.
 * @retval else Generic failure.
 */
int SE_Ini_MCU_FAN_Num(IN int enc_id, INOUT int *fan_num);

/**
 * @fn int SE_Set_MCU_Auto_Report(IN int enc_id, IN int mode)
 * @brief Set MCU auto report.
 *
 * This function will set MCU auto report.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] mode ON : 1 , OFF : 0.
 * @retval >= 0 Success.
 * @retval else Generic failure.
 */
int SE_Set_MCU_Auto_Report(IN int enc_id, IN int mode);

/**
 * @fn int SE_Ini_MCU_Temp_Num(IN int enc_id, INOUT int *temp_num)
 * @brief Initail TEMP number to MCU.
 *
 * This function will initail temp nubmer to MCU.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[inout] temp_num TEMP number.
 * @retval >= 0 Success.
 * @retval else Generic failure.
 */
int SE_Ini_MCU_Temp_Num(IN int enc_id, INOUT int *temp_num);

/**
 * @fn int SE_Upgrade_CPLD_FW(IN int enc_id, IN char *file_path)
 * @brief Upgrade CPLD FW using file_path.
 *
 * This function will upgrade CPLD FW using file_path.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] file_path The path of CPLD firmware.
 * @retval 0 Upgrade success.
 * @retval -1 Upgrade failed.
 */
int SE_Upgrade_CPLD_FW(IN int enc_id, IN char *file_path);

/**
 * @fn int SE_Set_TBT_Port_Power(IN int enc_id, IN int port_id, IN int power_on)
 * @brief Upgrade thunderbolt FW using file_path.
 *
 * This function will set the power status of the thunderbolt ports
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Thunderbolt slot id.
 * @param[in] power_on Force power ON/OFF the tbt ports.
 * @retval 0 Upgrade success.
 * @retval -1 Upgrade failed.
 */
int SE_Set_TBT_Port_Power(IN int enc_id, IN int port_id, IN int power_on);

/**
 * @fn int SE_Set_Disk_Data_Collection(IN int enable)
 * @brief Enable/disable the function of disk data collection.
 *
 * This function will enable/disable the function of disk data collection.
 * @param[in] enable Enable or disable of the function of disk data collection.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int SE_Set_Disk_Data_Collection(IN int enable);

/**
	for tl test
 */
void OnBnClickedButtonIDENTIFYSEP(char* dev);
void OnBnClickedButtonRdpage0(char* dev);
//void OnBnClickedButtonRdpage1(char* dev);
void OnBnClickedButtonRdpage2(char* dev);
void OnBnClickedButtonRdpage7(char* dev);
void OnBnClickedButtonSdpage2(char* dev);

/*@}*/  /* end of storage enclosure module */

/**
 * @defgroup HAL_PHYSICAL_DISK Physical Disk function
 */

/**
 * @ingroup HAL_PHYSICAL_DISK Physical Disk function
 */
/*@{*/  /* begin of physical disk module */

/**
 * @fn          typedef int (*PD_BOOT_ENUM_CALLBACK) (IN dev_t dev_no, IN void* contextP)
 * @brief       The function prototpye for physical disk boottime enumeration callback.
 * @param[in]   dev_no The device number (major, minor) of the block special file.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 * @retval      -2 Skip, callback function want to skip this identifier combination.
 */
typedef int (*PD_BOOT_ENUM_CALLBACK) (IN dev_t dev_no, IN void* contextP);

/**
 * @fn          typedef int (*PD_ENUM_CALLBACK) (IN int enc_id, IN int port_id, IN void* contextP)
 * @brief       The function prototpye of physical disk enumeration callback.
 * @param[in]   enc_id Enclosure identifier.
 * @param[in]   port_id Physical disk port identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 * @retval      -2 Skip, callback function want to skip this identifier combination.
 */
typedef int (*PD_ENUM_CALLBACK) (IN int enc_id, IN int port_id, IN void* contextP);

/**
 * @typedef PD_DEV_ID
 * @brief   Physical Disk logial identifier composed of enclosure and port identifier.
 */
typedef int PD_DEV_ID;

#define PD_ENC_ID(pd_dev_id)            ((0xffff0000 & pd_dev_id) >> 16)          /*!< Extract enclosure identifier from device identifier. */
#define PD_PORT_ID(pd_dev_id)           (0x0000ffff & pd_dev_id)                  /*!< Extract port identifier from device identifier. */
#define PD_MAKE_DEV_ID(enc_id, port_id) ((enc_id << 16) | (0x0000ffff & port_id)) /*!< Make up the device identifier from enclosure and port identifier. */

/**
 * @enum        PD_TYPE
 * @brief       Physical Disk Type.
 */
typedef enum _PD_TYPE
{
    PD_SATA = 0,                        /*!< SATA port. */
    PD_SAS,                             /*!< SAS port. */
    PD_USB,                             /*!< USB port. */
    PD_MMC,                             /*!< Memory Card port. */
    PD_NVME,                            /*!< NVME port. */
    PD_ISCSI,                           /*!< ISCSI. */
    PD_VIRTIO,                          /*! <VIRTIO block device. */
    PD_XENVBD,                          /*! <XEN virtual block device. */
} PD_TYPE;


/**
 * @enum        PD_FORM
 * @brief       Physical Disk Form Factor.
 */
typedef enum _PD_FORM
{
    PD_UNKNOWN_FORM = 0,                /*!< Unknown pd form. */
    PD_FORM_5_25,                       /*!< 5.25 inch port. */
    PD_FORM_3_5,                        /*!< 3.5 inch port. */
    PD_FORM_2_5,                        /*!< 2.5 inch port. */
    PD_FORM_1_8,                        /*!< 1.8 inch port. */
    PD_FORM_MIN,                        /*!< Less than 1.8 inch port. */
    PD_FORM_M_2_SATA,                   /*!< M.2 port, use SATA interface. */
    PD_FORM_M_2_PCIE,                   /*!< M.2 port, use PCIE interface. */
    PD_FORM_mSATA,                      /*!< mSATA port. */
    PD_FORM_PCIE,                       /*!< PCIE port. */
} PD_FORM;

/**
 * @enum        PD_DRIVER_TYPE
 * @brief       Physical Disk Driver Type.
 */
typedef enum _PD_DRIVER_TYPE
{
    PD_DRV_UNKNOWN = 0,                 /*!< Unknown driver type. */
    PD_DRV_USB,                         /*!< USB-like driver.  */
    PD_DRV_ATA,                         /*!< ATA-like driver.  */
    PD_DRV_SCSI,                        /*!< SCSI-like driver. */
    PD_DRV_SAS,                         /*!< SAS driver. */
    PD_DRV_FLASH,                       /*!< Flash driver,e.g NAND. */
    PD_DRV_MMC,                         /*!< MMC driver.*/
    PD_DRV_NVME,                        /*!< NVME driver.*/
    PD_DRV_M_2,                         /*!< virtual driver, combine PD_DRV_ATA and PD_DRV_NVME*/
    PD_DRV_VIRTIO,                      /*!< KVM VIRTIO driver. */
    PD_DRV_XENVBD,                      /*!< XEN VBD driver. */
} PD_DRIVER_TYPE;

/**
 * @name Physical disk capability bits
 */

/*@{*/  /* begin of PD_CAP */

#define PD_CAP_WWN                      0x00000001      /*!< WWN support. */
#define PD_CAP_SMART                    0x00000002      /*!< S.M.A.R.T support. */
#define PD_CAP_SELF_TEST                0x00000004      /*!< Self test support. */
#define PD_CAP_SSD                      0x00000008      /*!< Solid State Disk. */
#define PD_CAP_SERIAL_NO                0x00000010      /*!< With device serial no. */
#define PD_CAP_BOOT                     0x00000020      /*!< System boot disk. */
#define PD_CAP_PWR_STANDBY              0x00000040      /*!< Power standby mode. */
#define PD_CAP_DUMMY_DEVICE             0x00000080      /*!< Dummy disk. */
#define PD_CAP_ERROR_RECOVERY_CONTROL   0x00000100      /*!< ERROR_RECOVERY_CONTROL support. */
#define PD_CAP_TRIM                     0x00000200      /*!< Trim bit of DATA SET MANAGEMENT */
#define PD_CAP_CACHEABLE                0x00000400      /*!< PD is cacheable> */
#define PD_CAP_SECURITY_ERASE           0x00000800      /*!< Security erase support. */
#define PD_CAP_ODD                      0x00001000      /*!< CDROM - Optical Disc Device. */
#define PD_CAP_PHY_ERROR_PAGE           0x00002000      /*!< Phy error page supporte. */
#define PD_CAP_WWN_BLACKLIST            0x00004000      /*!< PD's WWN is Blacklisted. */
#define PD_CAP_TEMPERATURE_METHOD       0x00070000      /*!< Reserve 3 bits for temperature. */
#define PD_CAP_SATA_TEMP_SMART          0x00010000      /*!< Support temperature through SMART. */
#define PD_CAP_SATA_TEMP_SCT            0x00020000      /*!< Support temperature through SCT. */
#define PD_CAP_SATA_TEMP_DEV_STAT       0x00040000      /*!< Support temperature through Device Statistic. */
#define PD_CAP_TR_VIRTUAL_DEVICE        0x00080000      /*!< TR Vitrual Device */
#define PD_CAP_VIRTUAL_DEVICE           0x00400000      /*!< Vitrual Device */
#define PD_CAP_DZAT                     0x00100000      /*!< Deterministic Zeros after Trim */
#define PD_CAP_LIFE_USED_PERCENTAGE     0x00200000      /*!< Support SSD statistics. Life used percentage */
#define PD_CAP_READ_WRITE_BUFFER        0x10000000      /*!< Read/Write Buffer support. */
#define PD_CAP_READ_WRITE_BUFFER_DMA    0x20000000      /*!< Read/Write Buffer DMA support. */
#define PD_CAP_OPAL                     0x40000000      /*!< TCG OPAL 1.0/2.0 Support. */
#define PD_CAP_PM_VIRTUAL_DEVICE        0x80000000      /*!< Port Multiplier Vitrual Device */



/*@}*/  /* end of PD_CAP */

/**
 * @struct      PD_INFO
 * @brief       Physical Disk Information.
 */
typedef struct _PD_INFO {
    int port_id;                        /*!< Physical port id assigned by system dependent layer. */
    int32_t att_phy_id;                 /*!< Attached Phy id, -1: not supported */
    char pd_sys_id[MAX_SYS_ID_LEN];     /*!< System dependent physical disk identifier when communicating with the system dependent layer. */
    char enc_sys_id[MAX_SYS_ID_LEN];    /*!< System dependent enclosure identifier when communicating with the system dependent layer. */
    char pd_sys_name[MAX_PD_SYS_NAME];  /*!< System dependent disk name used in system utility. */
    char pd_ctrl_name[MAX_PD_CTRL_NAME];/*!< System dependent controller name used in system utility. */
    char pd_bus_name[MAX_PD_BUS_NAME];  /*!< System dependent bus name. */
    char wwn[MAX_WWN_LEN];              /*!< World Wide Name for this physical disk. */
    int wwn_len;                        /*!< World Wide Name length. */
    PD_TYPE type;                       /*!< Physical disk type. */
    PD_FORM form;                       /*!< Physical disk form factor. */
    PD_LINK_SPEED speed;                /*!< Current link speed. */
    PD_LINK_SPEED max_speed;            /*!< Host port max link speed. */
    PD_LINK_SPEED pd_max_speed;         /*!< Physical disk max link speed. */
    char pd_read_speed[32];
    char vendor[MAX_VENDOR_LEN];        /*!< Vendor name. */
    char model[MAX_MODEL_LEN];          /*!< Model name. */
    char serial_no[MAX_SERIAL_NO_LEN];  /*!< Disk unit serial number. */
    char revision[MAX_REVISION_LEN];    /*!< Disk revision. */
    unsigned int protocol_version;      /*!< Disk protocol version.bit:31-16:major, bit:15-0:minor */
    int status;                         /*!< Current enclosure status. */
    int capabilities;                   /*!< Physical disk capability support. */
    unsigned int sector_size;           /*!< Logical sector size. */
    unsigned long long capacity;        /*!< Disk capacity in terms of sector_size. */
    int rotation_speed;                 /*!< Disk rotation speed. -1:not reported, 0:SSD, else: rotation speed. */
    int hw_raid_disk_bitmap;            /*!< Port bitmap for the h/w raid(Virtual) disk in the TR(Turbo Raid) enclosure */
    int hw_raid_spare_bitmap;           /*!< Port bitmap for the spare drives of the h/w raid(Virtual) disk in the TR(Turbo Raid) enclosure */
    int hw_raid_type;                   /*!< Type the h/w raid(Virtual) disk in the TR(Turbo Raid) enclosure */
    char driver_name[MAX_DRIVER_NAME];  /*!< The kernel driver used for this pd */
    int mode;                           /*!< mode */
	//int sed_status;						
} PD_INFO;

/**
 * @enum        PD_POWER_STATUS
 * @brief       Physical disk power status.
 */
typedef enum _PD_POWER_STATUS
{
    PD_STATUS_STANDBY = 0,               /*!< Standby mode. */
    PD_STATUS_ACTIVE,                    /*!< Active mode. */
} PD_POWER_STATUS;

/**
 * @enum        PD_SELFTEST_MODE
 * @brief       Physical disk self test mode.
 */
typedef enum _PD_SELFTEST_MODE
{
    PD_SELFTEST_INVALID = -1,   /*!< the current self-test mode is invalid . */
    PD_SELFTEST_ABORT = 0,      /*!< Abort the current self-test running in background mode. */
    PD_SELFTEST_SHORT = 1,      /*!< The device server shall start its short self-test in the background mode. */
    PD_SELFTEST_EXTENDED = 2    /*!< The device server shall start its extended self-test in the background mode. */
} PD_SELFTEST_MODE;

/**
 * @enum        PD_ATA_SMART_RETURN_STATUS
 * @brief       ATA S.M.A.R.T RETURN STATUS
 */
typedef enum _PD_ATA_SMART_RETURN_STATUS
{
    PD_ATA_SMART_RETURN_INVALID = -1,   /*!< the return status is invalid . */
    PD_ATA_SMART_RETURN_NORMAL = 0,     /*!< normal status . */
    PD_ATA_SMART_RETURN_WARM = 1,       /*!< threshold exceed */
} PD_ATA_SMART_RETURN_STATUS;

/**
 * @enum        PD_ATA_SELFTEST_STATUS
 * @brief       ATA Physical disk self-test status.
 */
typedef enum _PD_ATA_SELFTEST_STATUS
{
    PD_ATA_TEST_INVALID = -1,           /*!< Invalid status. */
    PD_ATA_TEST_COMPLETED = 0,          /*!< Normal status. */
    PD_ATA_TEST_ABORTED = 1,            /*!< Warning status. */
    PD_ATA_TEST_INETRRUPTED = 2,        /*!< The self-test routine was interrupted. */
    PD_ATA_TEST_FATAL_ERR = 3,          /*!< A fatal error or unknown test error occurred. */
    PD_ATA_TEST_UNKNOWN_FAILED = 4,     /*!< The previous self-test completed having an unknown test failed. */
    PD_ATA_TEST_EE_FAILED = 5,          /*!< The previous self-test completed having the electrical element of the test failed. */
    PD_ATA_TEST_SERVO_FAILED = 6,       /*!< The previous self-test completed having the servo test element of the test failed. */
    PD_ATA_TEST_READ_FAILED = 7,        /*!< The previous self-test completed having the read element of the test failed. */
    PD_ATA_TEST_DEVICE_DAMAGE = 8,      /*!< The previous self-test completed having a test element that failed and the device is suspected of having damage. */
    PD_ATA_TEST_IN_PROGRESS = 15        /*!< Self-test routine in progress. */
} PD_ATA_SELFTEST_STATUS;

/**
 * @enum		PD_SCSI_SELFTEST_STATUS
 * @brief		SCSI Physical disk self-test status.
 */
typedef enum _PD_SCSI_SELFTEST_STATUS
{
    PD_SCSI_TEST_INVALID = -1,      /*!< Invalid Status */
    PD_SCSI_TEST_COMPLETED = 0,     /*!< The self-test completed without error. */
    PD_SCSI_TEST_ABORTED = 1,       /*!< The background self-test was aborted by application client using a SEND DIAGNOSTIC cmd. */
    PD_SCSI_TEST_ABORTED_MISC = 2,  /*!< The background self-test was aborted by application client using a method other than a SEND DIAGNOSTIC cmd. */
    PD_SCSI_TEST_UNKNOWN_ERROR = 3, /*!< An unknown error occurred while the device server was processing the self-test and the device server was unable to complete the self-test. */
    PD_SCSI_UNKNOWN_SEGMENT_FAILED = 4, /*!< The self-test completed with a failure in a test segment, and the test segment that failed is not known. */
    PD_SCSI_1ST_SEGMENT_FAILED = 5, /*!< The first segment of the self-test failed. */
    PD_SCSI_2ND_SEGMENT_FAILED = 6, /*!< The second segment of the self-test failed. */
    PD_SCSI_OTHER_SEGMENT_FAILED = 7,   /*!< Another segment of the self-test failed and which test is indicated by the contents of the SELF-TEST NUMBER field. */
    PD_SCSI_TEST_IN_PROGRESS = 15   /*!< The self-test is in progress. */
} PD_SCSI_SELFTEST_STATUS;

/**
 * @struct      PD_SCSI_LOG_PAGE
 * @brief       SCSI/SAS HDD S.M.A.R.T like Information, please refer section
 *              6.54 of ATA specification volume one.
 */
typedef struct _PD_SCSI_LOG_PAGE {

    unsigned int page_code;                                     /*!< Log page code. */

    union {
        // for the following log page:
        // SPC 7.3.14 Read Error Counter
        // SPC 7.3.22 Verify Error Counter
        // SPC 7.3.23 Write Error Counter
        struct _err_counter {
            const char* corrected_wo_delay_str;                 /*!< Errors corrected without substantial delay.*/
            unsigned int corrected_wo_delay;                    /*!< Errors corrected without substantial delay. */
            const char* corrected_w_delay_str;                  /*!< Errors corrected with possible delays.*/
            unsigned int corrected_w_delay;                     /*!< Errors corrected with possible delays. */
            const char* total_error_str;                        /*!< Total (e.g., rewrites or rereads)*/
            unsigned int total_error;                           /*!< Total (e.g., rewrites or rereads) */
            const char* total_corrected_str;                    /*!< Total errors corrected.*/
            unsigned int total_corrected;                       /*!< Total errors corrected. */
            const char* total_time_corrected_str;               /*!< Total times correction algorithm processed.*/
            unsigned int total_time_corrected;                  /*!< Total times correction algorithm processed. */
            const char* total_bytes_str;                        /*!< Total times correction algorithm processed.*/
            unsigned long long total_bytes;                     /*!< Total times correction algorithm processed. */
            const char* total_uncorrected_str;                  /*!< Total uncorrected errors.*/
            unsigned int total_uncorrected;                     /*!< Total uncorrected errors. */
        } err_counter;

        // SPC 7.3.11 Non-medium error log page
        struct _non_medium_error {
            const char* non_medium_err_str;                     /*!< Non-medium error count.*/
            unsigned int non_medium_err;                        /*!< Non-medium error count. */
        } non_medium_error;


        // SPC 7.3.17 Start-Stop Cycle Counter log page
        struct _start_stop_cycle_counter {
            const char* date_of_man_str;                        /*!< Date of Manufacture.*/
            unsigned char date_of_man[8];                       /*!< Date of Manufacture. */
            const char* account_date_str;                       /*!< Accounting Date.*/
            unsigned char account_date[8];                      /*!< Accounting Date. */
            const char* cycle_count_over_lifetime_str;          /*!< Specified Cycle Count Over Device Lifetime.*/
            unsigned int cycle_count_over_lifetime;             /*!< Specified Cycle Count Over Device Lifetime.*/
            const char* accum_start_stop_cycle_str;             /*!< Accumulated Start-Stop Cycles.*/
            unsigned int accum_start_stop_cycle;                /*!< Accumulated Start-Stop Cycles. */
            const char* load_unload_count_over_lifetime_str;    /*!< Specified Load-Unload Count Over Device Lifetime.*/
            unsigned int load_unload_count_over_lifetime;       /*!< Specified Load-Unload Count Over Device Lifetime. */
            const char* accum_load_unload_cycle_str;            /*!< Accumulated Load-Unload Cycles.*/
            unsigned int accum_load_unload_cycle;               /*!< Accumulated Load-Unload Cycles. */
        } start_stop_cycle_counter;

        // SPC 7.3.21 Temperature log page
        struct _temperature {
            const char* temp_str;                               /*!< Temperature.*/
            unsigned int temp;                                  /*!< Temperature. */
            const char* ref_temp_str;                           /*!< Reference Temperature.*/
            unsigned int ref_temp;                              /*!< Reference Temperature. */
        } temperature;

        // SPC 7.3.8.2 Informational Exceptions log page
        struct _informaion_exception {
            const char* asc_str;                                /*!< Sense Code.*/
            unsigned char asc;                                  /*!< Sense Code. */
            const char* ascq_str;                               /*!< Sense Code Qualifier.*/
            unsigned char ascq;                                 /*!< Sense Code Qualifier. */
        } informaion_exception;

        // SBC 6.4.2.2 Background Scan Status log parameter
        struct _accumulated_power_on {
            const char* hours_str;                              /*!< power on hours string.*/
            unsigned int hours;                                 /*!< power on hours */
        } accumulated_power_on;
    } page_param; /*!< Log page parameters according to the different page. */
} PD_SCSI_LOG_PAGE;

/**
 * @struct      PD_NVME_SMART_LOG_PAGE
 * @brief       NVME HDD S.M.A.R.T like Information, please refer section
 *              5.10.1 Log Specific Information of NVM-Express-1_2a.pdf
 */
typedef struct _PD_NVME_SMART_LOG_PAGE {
    unsigned char critical_warning;
    unsigned char temperature[2];
    unsigned char avail_spare;
    unsigned char spare_thresh;
    unsigned char percent_used;
    unsigned char rsvd6[26];
    unsigned char data_units_read[16];
    unsigned char data_units_written[16];
    unsigned char host_reads[16];
    unsigned char host_writes[16];
    unsigned char ctrl_busy_time[16];
    unsigned char power_cycles[16];
    unsigned char power_on_hours[16];
    unsigned char unsafe_shutdowns[16];
    unsigned char media_errors[16];
    unsigned char num_err_log_entries[16];
    unsigned int warning_temp_time;
    unsigned int critical_comp_time;
    unsigned short temp_sensor[8];
    unsigned char rsvd216[296];
} PD_NVME_SMART_LOG_PAGE;

typedef struct _PD_NVME_SELF_TEST_RES {
  unsigned char device_self_test_status;
  unsigned char segment_num;
  unsigned char valid_diagnostic_info;
  unsigned char rsvd;
  unsigned long int power_on_hours;
  unsigned int nsid;
  unsigned long int failing_lba;
  unsigned char status_code_type;
  unsigned char status_code;
  unsigned char vendor_specific[2];
}  __attribute__((packed)) PD_NVME_SELF_TEST_RES;

typedef struct _PD_NVME_SELF_TEST_LOG {
  unsigned char  crnt_dev_selftest_oprn;
  unsigned char  crnt_dev_selftest_compln;
  unsigned char  rsvd[2];
  PD_NVME_SELF_TEST_RES result[20];
}  __attribute__((packed)) PD_NVME_SELF_TEST_LOG; 

/**
 * @struct      PD_NVME_ERROR_LOG_PAGE
 * @brief       NVME HDD ERROR LOG like Information, please refer section
 *              5.14.1.1 Log Specific Information of NVM-Express-1_3.pdf
 */
typedef struct _PD_NVME_ERROR_LOG_PAGE {
    unsigned long long error_count;
    unsigned short sqid;
    unsigned short cmdid;
    unsigned short status_field;
    unsigned short parm_error_location;
    unsigned long long lba;
    unsigned int nsid;
    unsigned char vs;
    unsigned char resv[3];
    unsigned long long cs;
    unsigned char resv2[24];
}PD_NVME_ERROR_LOG_PAGE;


/**
 * @struct      nvme_id_power_state
 * @brief       nvme identify power state data structure
 *
 */
typedef struct _PD_NVME_ID_POWER_STATE {
	unsigned char	max_power[2];	/* centiwatts */
	unsigned char	rsvd2;
	unsigned char	flags;
	unsigned char	entry_lat[4];	/* microseconds */
	unsigned char	exit_lat[4];	/* microseconds */
	unsigned char	read_tput;
	unsigned char	read_lat;
	unsigned char	write_tput;
	unsigned char	write_lat;
	unsigned char	idle_power[2];
	unsigned char	idle_scale;
	unsigned char	rsvd19;
	unsigned char	active_power[2];
	unsigned char	active_work_scale;
	unsigned char	rsvd23[9];
} PD_NVME_ID_POWER_STATE;

struct nvme_lbaf {
    unsigned char ms[2];
    unsigned char ds;
    unsigned char rp;
};

/**
 * @struct      nvme_id_ns
 * @brief       nvme identify namespace data structure
 *
 */
typedef struct _NVME_ID_NS {
    unsigned char nsze[8];
    unsigned char ncap[8];
    unsigned char nuse[8];
    unsigned char nsfeat;
    unsigned char nlbaf;
    unsigned char flbas;
    unsigned char mc;
    unsigned char dpc;
    unsigned char dps;
    unsigned char nmic;
    unsigned char rescap;
    unsigned char fpi;
    unsigned char rsvd33;
    unsigned char nawun[2];
    unsigned char nawupf[2];
    unsigned char nacwu[2];
    unsigned char nabsn[2];
    unsigned char nabo[2];
    unsigned char nabspf[2];
    unsigned char rsvd46[2];
    unsigned char nvmcap[16];
    unsigned char rsvd64[40];
    unsigned char nguid[16];
    unsigned char eui64[8];
    struct nvme_lbaf lbaf[16];
    unsigned char rsvd192[192];
    unsigned char vs[3712];
} NVME_ID_NS;

/**
 * @struct      nvme_id_ctrl 
 * @brief       nvme identify controller data structure
 *
 */
typedef struct _PD_NVME_ID_CTRL {
	unsigned char	vid[2];
	unsigned char	ssvid[2];
	char			sn[20];
	char			mn[40];
	char			fr[8];
	unsigned char	rab;
	unsigned char	ieee[3];
	unsigned char	cmic;
	unsigned char	mdts;
	unsigned char	cntlid[2];
	unsigned char	ver[4];
	unsigned char	rtd3r[4];
	unsigned char	rtd3e[4];
	unsigned char	oaes[4];
	unsigned char	ctratt[4];
	unsigned char	rsvd100[156];
	unsigned char	oacs[2];
	unsigned char	acl;
	unsigned char	aerl;
	unsigned char	frmw;
	unsigned char	lpa;
	unsigned char	elpe;
	unsigned char	npss;
	unsigned char	avscc;
	unsigned char	apsta;
	unsigned char	wctemp[2];
	unsigned char	cctemp[2];
	unsigned char	mtfa[2];
	unsigned char	hmpre[4];
	unsigned char	hmmin[4];
	unsigned char	tnvmcap[16];
	unsigned char	unvmcap[16];
	unsigned char	rpmbs[4];
	unsigned char	edstt[2];
	unsigned char	dsto;
	unsigned char	fwug;
	unsigned char	kas[2];
	unsigned char	hctma[2];
	unsigned char	mntmt[2];
	unsigned char	mxtmt[2];
	unsigned char	sanicap[4];
	unsigned char	hmminds[4];
	unsigned char	hmmaxd[2];
	unsigned char	rsvd338[174];
	unsigned char	sqes;
	unsigned char	cqes;
	unsigned char	maxcmd[2];
	unsigned char	nn[4];
	unsigned char	oncs[2];
	unsigned char	fuses[2];
	unsigned char	fna;
	unsigned char	vwc;
	unsigned char	awun[2];
	unsigned char	awupf[2];
	unsigned char	nvscc;
	unsigned char	rsvd531;
	unsigned char	acwu[2];
	unsigned char	rsvd534[2];
	unsigned char	sgls[4];
	unsigned char	rsvd540[228];
	char			subnqn[256];
	unsigned char	rsvd1024[768];
	unsigned char	ioccsz[4];
	unsigned char	iorcsz[4];
	unsigned char	icdoff[2];
	unsigned char	ctrattr;
	unsigned char	msdbd;
	unsigned char	rsvd1804[244];
	PD_NVME_ID_POWER_STATE	psd[32];
	unsigned char	vs[1024];
}PD_NVME_ID_CTRL;

// Disk data package
typedef enum _DISK_DATA_TEMPLATE
{
    DISK_DATA_TEMPLATE_IDENTIFY                             = 0,
    DISK_DATA_TEMPLATE_SMART_ATTR                           = 1,
    DISK_DATA_TEMPLATE_EXTENDED_COMPREHENSIVE_SMART_ERR_LOG = 2,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE1                  = 3,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE2                  = 4,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE3                  = 5,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE4                  = 6,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE5                  = 7,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE6                  = 8,
    DISK_DATA_TEMPLATE_DEV_STATISTIC_PAGE7                  = 9,
    DISK_DATA_TEMPLATE_SUMMARY_SMART_ERR_LOG                = 10,
    DISK_DATA_TEMPLATE_COMPREHENSIVE_SMART_ERR_LOG          = 11,
    DISK_DATA_TEMPLATE_SMART_ATTRIBUTE_THRESHOLD            = 12,
    DISK_DATA_TEMPLATE_SMART_SELF_TEST_LOG                  = 13,
    DISK_DATA_TEMPLATE_EXTENDED_SMART_SELF_TEST_LOG         = 14,
    DISK_DATA_TEMPLATE_SATA_INTERFACE_STATISTICS            = 15,
    DISK_DATA_TEMPLATE_HOST_DRIVER_STATISTICS               = 21,
    DISK_DATA_TEMPLATE_DRIVER_FAILURE_LOG                   = 22,
} DISK_DATA_TEMPLATE;

typedef enum _NVME_DATA_TEMPLATE
{
    TEMPLATE_NVME_IDENTIFY_CTRL                             = 0,
    TEMPLATE_NVME_IDENTIFY_NAMESPACE                        = 1,
    TEMPLATE_NVME_SMART_LOG                                 = 2,
    TEMPLATE_NVME_ERR_LOG                                   = 3,
    TEMPLATE_NVME_SELF_TEST                                 = 4,
    TEMPLATE_RAID_INFO                                      = 5,
    TEMPLATE_NVME_MAX_ITEMS                                 = 6,
} NVME_DATA_TEMPLATE;


#define QNAP_DRIVE_ANALYZER         "Header: QNAP Drive Analyzer\n"
#define QNAP_DRIVE_ANALYZER_VERSION "1.00"

#define MAX_ATTRIBUTE_COUNT             30
#define MAX_SATA_DISK_DATA_TEMPLATE     23
#define DISK_DATA_PACKAGE_HEADER_SIZE   0x800
#define DISK_DATA_PATH "/tmp/smart/disk_data_pkg_%d-%d.bin"
// Disk data package

/**
 * @fn int PD_Get_Temperature(IN int enc_id, IN int port_id,
 *         IN int direct, OUT int temp)
 * @brief Retrieve the current test type from config file.
 *
 * TODO support SATA only now
 * This function will get temperature of specific disk through sct status.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] direct Access temperature directly. TODO: not support
 * @param[out] temp The temperature in 'C.
 *
 * @retval 0, successful.
 * @retval -1, error.
 */
int PD_Get_Temperature(IN int enc_id, IN int port_id,
        IN int direct, OUT int *temp);

/**
 * @fn int PD_Get_SR_Temperature(IN int enc_id, IN int port_id, IN int member_id, 
 *         IN int direct, OUT int temp)
 * @brief Retrieve the temperature through SMART.
 *
 * This function will get temperature of specific disk through SMART.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] direct Access temperature directly(only support direct=1 for now).
 * @param[out] temp The temperature in 'C.
 *
 * @retval 0, successful.
 * @retval -1, error.
 */
int PD_Get_SR_Temperature(IN int enc_id, IN int port_id, IN int member_id, 
        IN int direct, OUT int *temp);

/**
 * @fn int PD_Boot_Enumerate(OUT dev_t dev_no_ary[], IN unsigned int dev_no_ary_count,
 *                           IN PD_BOOT_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate all available physical disks during boottime.
 *
 * This function is called by nas boot utility to obtain all available device number of physical disks.
 * @param[out] dev_no_ary A dev_t arrary to host the available device number.
 * @param[in] dev_no_ary_count The entry count of the dev_t array.
 * @param[in] callback Physical disk boot enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available physical disk, if the count is larger than
 *         dev_no_ary_count, the calling utility should allocate more space for device number array to
 *         obtain all available disk device number.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int PD_Boot_Enumerate(OUT dev_t dev_no_ary[], IN unsigned int dev_no_ary_count,
                      IN PD_BOOT_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int PD_Enumerate_By_Cap(IN int enc_id, IN int capabilities, IN int match_rule, OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN PD_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available disks which match the capabilities for specified enclosure from the
 *        internal configuration file.
 *
 * This function is called by nas utility to obtain available port identifiers of physical disk
 * which match the specified capabilities for some enclosure in the internal configuration file.
 * @param[in] enc_id Specified the enclosure logical identifier.
 * @param[in] capabilities The capabilities bit to be matched.
 * @param[in] match_rule The matching rule of the capabilites:
 *            0: match none
 *            1: match any one
 *            2: match all
 * @param[out] id_ary An identifier array to host the available port identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Physical disk enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available physical disk, if the count is larger than
 *         id_ary_count, the calling utility should allocate more space for identifier array to
 *         obtain all available port identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int PD_Enumerate_By_Cap(IN int enc_id, IN int capabilities, IN int match_rule, OUT int id_ary[], IN unsigned int id_ary_count,
                  IN PD_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int PD_Enumerate(IN int enc_id, OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN PD_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available physical disks for specified enclosure from the
 *        internal configuration file.
 *
 * This function is called by nas utility to obtain available port identifiers of physical disk
 * for some enclosure in the internal configuration file.
 * @param[in] enc_id Specified the enclosure logical identifier.
 * @param[out] id_ary An identifier array to host the available port identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Physical disk enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available physical disk, if the count is larger than
 *         id_ary_count, the calling utility should allocate more space for identifier array to
 *         obtain all available port identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int PD_Enumerate(IN int enc_id, OUT int id_ary[], IN unsigned int id_ary_count,
                 IN PD_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int PD_Enumerate_All(OUT PD_DEV_ID id_ary[], IN unsigned int id_ary_count,
 *                          IN PD_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate all available physical disks for all enclosures from the
 *        internal configuration file.
 *
 * This function is called by nas utility to obtain all available physical disks
 * in the internal configuration file.
 * @param[out] id_ary An PD_DEV_ID array to host all available PD_DEV_IDs.
 * @param[in] id_ary_count The entry count of the PD_DEV_ID array.
 * @param[in] callback Physical disk enumeration all callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of all available physical disks, if the count is larger than
 *         id_ary_count, the calling utility should allocate more space for identifier array to
 *         obtain all available port identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int PD_Enumerate_All(OUT PD_DEV_ID id_ary[], IN unsigned int id_ary_count,
                     IN PD_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int PD_Get_Info(IN int enc_id, IN int port_id, OUT PD_INFO *pd_infoP)
 * @brief Retrieve the specified physical disk from the internal configuration file.
 *
 * This function is called by nas utility to obtain the specified physical disk information
 * from the internal configuration file.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] pd_infoP a PD_INFO structure pointer to host the physical disk information.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Info(IN int enc_id, IN int port_id, OUT PD_INFO *pd_infoP);

/**
 * @fn int VD_Enumerate(IN int enc_id, OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN PD_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available virtual disks for specified enclosure from the 
 *        internal configuration file.
 *
 * This function is called by nas utility to obtain available port identifiers of virtual disk 
 * for some enclosure in the internal configuration file (only for model TR-00x).
 * @param[in] enc_id Specified the enclosure logical identifier.
 * @param[out] id_ary An identifier array to host the available port identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Physical disk enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available physical disk, if the count is larger than
 *         id_ary_count, the calling utility should allocate more space for identifier array to
 *         obtain all available port identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int VD_Enumerate(IN int enc_id, OUT int id_ary[], IN unsigned int id_ary_count,
                 IN PD_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int VD_Get_Info(IN int enc_id, IN int vd_id, OUT PD_INFO *pd_infoP)
 * @brief Retrieve the specified virtual disk from the internal configuration file.
 *
 * This function is called by nas utility to obtain the specified virtual disk information
 * from the internal configuration file.
 * @param enc_id Logical enclosure identifier.
 * @param vd_id Virtual disk ID in enclosure_x.conf. 
 * @param pd_infoP a PD_INFO structure pointer to host the physical disk information.
 * @retval 0 Success, the specified physical disk is retrieved completed.
 * @retval -1 Generic failure, Fail to retrieve the specified physical disk in the 
 *         internal configurationturn file.
 */
int VD_Get_Info(IN int enc_id, IN int vd_id, OUT PD_INFO *pd_infoP);

/**
 * @fn int PD_Get_Id_By_Sys_Id(IN char* enc_sys_id, IN char* pd_sys_id, OUT int* enc_idP, OUT int* port_idP)
 * @brief Retrieve the logical physical disk identifier via system dependent identifier.
 *
 * This function is called by PnP utility to obtain the assigned logical identifier from the
 * internal configuration file.
 * @param[in] enc_sys_id enclosure system dependent identifier.
 * @param[in] pd_sys_id physical disk system dependent identifier.
 * @param[out] enc_idP a logical enclosure identifier that has been assigned to this disk.
 * @param[out] port_idP a logical port identifier that has been assigned to this disk.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified physical disk identifier in the
 *         internal configurationturn file.
 */
int PD_Get_Id_By_Sys_Id(IN char* enc_sys_id, IN char* pd_sys_id, OUT int* enc_idP, OUT int* port_idP);

/**
 * int PD_Get_Sys_Id_By_Port_Id(IN char* enc_sys_id, IN int port_id,
 *         OUT char* pd_sys_id, IN unsigned int buf_len)
 * @brief Get PD sys id by port id.
 *
 * This function will get pd sys id by port id in enc_id.
 * @param[in] enc_sys_id Enclosure id.
 * @param[in] port_id Physical disk port identifer.
 * @param[OUT] pd_sys_id PD sys id.
 * @param[OUT] buf_len length of pd sys id.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int PD_Get_Sys_Id_By_Port_Id(IN char* enc_sys_id, IN int port_id,
        OUT char* pd_sys_id, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Id_By_Sys_Id_From_History(IN char* enc_sys_id, IN char* pd_sys_id, OUT int* enc_idP, OUT int* port_idP)
 * @brief Retrieve the logical physical disk identifier in the backuped configuration file via system dependent identifier.
 *
 * This function is called by PnP utility to obtain the assigned logical identifier from the internal backuped configuration file.
 * @param[in] enc_sys_id enclosure system dependent identifier.
 * @param[in] pd_sys_id physical disk system dependent identifier.
 * @param[out] enc_idP a logical enclosure identifier that has been assigned to this disk.
 * @param[out] port_idP a logical port identifier that has been assigned to this disk.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified physical disk identifier in the
 *         internal configurationturn file.
 */
int PD_Get_Id_By_Sys_Id_From_History(IN char* enc_sys_id, IN char* pd_sys_id, OUT int* enc_idP, OUT int* port_idP);

/**
 * @fn int PD_Get_Id_By_Sys_Name(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP)
 *
 * @brief Retrieve the PD_DEV_ID by PD system name.
 *
 * This function will return enclosure and port identifier for the specified PD system name.
 * @param[in] pd_sys_name  System dependent disk name used in system utility.
 * @param[out] enc_idP Enclosure identifier.
 * @param[out] port_idP Port identifier.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Id_By_Sys_Name(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP);

/**
 * @fn int PD_Get_Id_By_Sys_Name_From_History(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP)
 *
 * @brief Retrieve the PD_DEV_ID in the backuped configuration file by PD system name.
 *
 * This function will return enclosure and port identifier recorded in the backuped configuration file for the specified PD system name.
 * @param[in] pd_sys_name  System dependent disk name used in system utility.
 * @param[out] enc_idP Enclosure identifier.
 * @param[out] port_idP Port identifier.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Id_By_Sys_Name_From_History(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP);

/**
 * @fn int PD_Get_Id_By_Enc_Conf(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP)
 *
 * @brief Retrieve the PD_DEV_ID by PD system name from the existed enclosure_x.conf.
 *
 * This function will return enclosure and port identifier for the specified PD system name by checking the configuration file and the backuped one.
 * @param[in] pd_sys_name  System dependent disk name used in system utility.
 * @param[out] enc_idP Enclosure identifier.
 * @param[out] port_idP Port identifier.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Id_By_Enc_Conf(IN char* pd_sys_name, OUT int* enc_idP, OUT int* port_idP);

/**
 * @fn int PD_Get_Id_By_Dev_No(IN dev_t dev_no, OUT int* enc_idP, OUT int* port_idP)
 *
 * @brief Retrieve the PD_DEV_ID by PD device no.
 *
 * This function will return enclosure and port identifier for the specified PD device no.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @param[out] enc_idP Enclosure identifier.
 * @param[out] port_idP Port identifier.
 * @retval 0 Success, the specified physical disk identifier is retrieved completely.
 * @retval -1 Generic failure.
 * @note This function is only valid after SE_Scan being called.
 */
int PD_Get_Id_By_Dev_No(IN dev_t dev_no, OUT int* enc_idP, OUT int* port_idP);

/**
 * @fn int PD_Get_Dev_No_By_Id(IN int enc_id, IN int port_id, OUT dev_t* dev_noP)
 *
 * @brief Retrieve the device number PD device identifier.
 *
 * This function will return the device number for the specified PD device identifer.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] dev_noP PD device number.
 * @retval 0 Success, the specified physical disk device number is retrieved completely.
 * @retval -1 Generic failure.
 * @note This function is only valid after SE_Scan being called.
 */
int PD_Get_Dev_No_By_Id(IN int enc_id, IN int port_id, OUT dev_t* dev_noP);

/**
 * @fn int PD_Get_Dev_No_By_Sys_Name(IN char* pd_sys_name, OUT dev_t* dev_noP)
 *
 * @brief Retrieve the device number by PD system name.
 *
 * This function will return device number (major, minor) for the specified PD system name.
 * @param[in] pd_sys_name  System dependent disk name used in system utility.
 * @param[out] dev_noP Disk device number.
 * @retval 0 Success, the specified physical disk device number is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Dev_No_By_Sys_Name(IN char* pd_sys_name, OUT dev_t* dev_noP);

/**
 * @fn int PD_Scan(IN int enc_id)
 * @brief Scan the connected storage enclosure, enumerate and store all
 *        available physical disks in the internal configuration file.
 *
 * This function will remove the original physical disk settings in the
 * internal configuration file and invoke the system dependent enumerate
 * function to re-generate them.
 * @param[in] enc_id
 * @retval Return the count of the available physical disks.
 */
int PD_Scan(IN int enc_id);

/**
 * @fn int PD_Get_Boot_Disk_Serial_No(IN int disk_id, OUT char* serial_no, IN unsigned int buf_len)
 * @brief Retrieve the serial number for the bootable disk.
 *
 * This function is called by system utility to obtain the serial number for specified boot disk.
 * @param[in] disk_id  Boot disk index (0 based).
 * @param[out] serial_no Serial number of the specified boot disk.
 * @param[in] buf_len The buffer count for the serial_no.
 * @retval 0 Success, the specified boot disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the serial number for specified boot disk in the
 *         internal configurationturn file.
 */
int PD_Get_Boot_Disk_Serial_No(IN int disk_id, OUT char* serial_no, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Boot_Disk(IN int disk_id, OUT char* disk_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the bootable disk.
 *
 * This function is called by system utility to obtain the system dependent name for specified boot disk.
 * @param[in] disk_id  Boot disk index (0 based).
 * @param[out] disk_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified boot disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified boot disk in the
 *         internal configurationturn file.
 */
int PD_Get_Boot_Disk(IN int disk_id, OUT char* disk_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Boot_Disk_At_Startup(IN int disk_id, OUT char* disk_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the bootable disk by hal_sys scan
 *
 * This function is called by system utility to obtain the system dependent name for specified boot disk at booting time.
 * @param[in] disk_id  Boot disk index (0 based).
 * @param[out] disk_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified boot disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified boot disk in the internal configurationturn file.
 */
int PD_Get_Boot_Disk_At_Startup(IN int disk_id, OUT char* disk_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Boot_Disk_Capacity(IN int disk_id,
 *                                   OUT unsigned long long* capacityP, OUT unsigned int* sector_sizeP)
 * @brief Retrieve the specified boot disk capacity.
 *
 * This function will retrieve the capacity and sector size for the specified boot disk.
 * @param[in] disk_id  Boot disk index (0 based).
 * @param[out] capacityP The physical disk capacity in terms of sector.
 * @param[out] sector_sizeP The sector size of the specified physical disk.
 * @retval 0 Success, the specified physical disk capacity is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Boot_Disk_Capacity(IN int disk_id, OUT unsigned long long* capacityP, OUT unsigned int* sector_sizeP);

/**
 * @fn int PD_Is_Exist(IN int enc_id, IN int port_id)
 * @brief Check if the specified physical disk exist or not.
 *
 * This function is called by system utility to obtain the existence of the specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @retval 1 TRUE, the specified physical disk exist.
 * @retval 0 FALSE, the specified physical disk doesn't exist.
 */
int PD_Is_Exist(IN int enc_id, IN int port_id);

/**
 * @fn int VD_Is_Exist(IN int enc_id, IN int port_id)
 * @brief Check if the specified virtual disk exist or not.
 *
 * This function is called by system utility to obtain the existence of the specified virtual disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Virutal disk port id assigned by system dependent layer (1 based). 
 * @retval 1 TRUE, the specified physical disk exist.
 * @retval 0 FALSE, the specified physical disk doesn't exist.
 */
int VD_Is_Exist(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Is_Exist_By_Sys_Name(IN char* sys_name)
 * @brief Check if the specified physical disk exist or not via the PD system name.
 *
 * This function is called by system utility to obtain the existence of the specified physical disk.
 * @param[in] sys_name Physical disk system name.
 * @retval 1 TRUE, the specified physical disk exist.
 * @retval 0 FALSE, the specified physical disk doesn't exist.
 */
int PD_Is_Exist_By_Sys_Name(IN char* sys_name);

/**
 * @fn int PD_Get_Serial_No(IN int enc_id, IN int port_id, OUT char* serial_no, IN unsigned int buf_len)
 * @brief Retrieve the serial number for the specified device number of physical disk.
 *
 * This function is called by boot utility to obtain the serial number for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] serial_no Disk serial number.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Serial_No(IN int enc_id, IN int port_id, OUT char* serial_no, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Serial_No_By_Dev_No(IN int enc_id, IN int port_id, OUT char* serial_no, IN unsigned int buf_len)
 * @brief Retrieve the serial number for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the serial number for specified physical disk.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] serial_no Disk serial number.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Serial_No_By_Dev_No(IN dev_t dev_no, OUT char* serial_no, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Vendor(IN int enc_id, IN int port_id, OUT char* vendor, IN unsigned int buf_len)
 * @brief Retrieve the vendor name for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the vendor name for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based). 
 * @param[out] vendor Disk vendor name.
 * @param[in] buf_len The buffer count for the vendor name.
 * @retval 0 Success, the vendor name for specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the vendor name for specified physical disk in the 
 *         internal configurationturn file.
 */
int PD_Get_Vendor(IN int enc_id, IN int port_id, OUT char* vendor, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Model(IN int enc_id, IN int port_id, OUT char* model, IN unsigned int buf_len)
 * @brief Retrieve the model name for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the model name for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] model Disk model name.
 * @param[in] buf_len The buffer count for the model name.
 * @retval 0 Success, the model name for specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the model name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Model(IN int enc_id, IN int port_id, OUT char* model, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Sys_Name(IN int enc_id, IN int port_id, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the system dependent name for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Sys_Name(IN int enc_id, IN int port_id, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Sys_Name_From_History(IN int enc_id, IN int port_id, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for specified physical disk from the internal backuped configuration file.
 *
 * This function is called by system utility to obtain the system dependent name for specified physical disk from the backeduped configuration file.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Sys_Name_From_History(IN int enc_id, IN int port_id, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Slot_Name(IN int enc_id, IN int port_id, OUT char* bufP, int buf_len)
 *
 * @brief Retrieve the slot name of the specified disk port.
 *
 * This function will return predefined slot name of the specified disk port from model.conf.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] bufP buffer pointer to store returned slot name.
 * @param[in] buf_len length of the input buffer.
 * @retval 0 Success, the specified slot name is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Slot_Name(IN int enc_id, IN int port_id, OUT char* bufP, int buf_len);

/**
 * @fn int PD_Get_USB_Disk_Sys_Name(IN int port_id, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified USB port of root enclosure.
 *
 * This function is called by system utility to obtain the system dependent name for specified USB port.
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified USB port is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified USB port in the
 *         internal configurationturn file.
 */
int PD_Get_USB_Disk_Sys_Name(IN int port_id, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_eSATA_Disk_Sys_Name(IN int port_id, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified eSATA port of root enclosure.
 *
 * This function is called by system utility to obtain the system dependent name for specified eSATA port.
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified eSATA port is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified USB port in the
 *         internal configurationturn file.
 */
int PD_Get_eSATA_Disk_Sys_Name(IN int port_id, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Sys_Name_By_Dev_No(IN dev_t dev_no, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified device number of physical disk.
 *
 * This function is called by boot utility to obtain the system name for specified physical disk.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @param[out] sys_name System name recognized by Operating System.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified device number.
 */
int PD_Get_Sys_Name_By_Dev_No(IN dev_t dev_no, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_WWN(IN int enc_id, IN int port_id, OUT char* wwn, IN unsigned int buf_len)
 * @brief Retrieve the WWN for the specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the WWN of the specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] wwn WWN returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completed.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_WWN(IN int enc_id, IN int port_id, OUT char* wwn, IN unsigned int buf_len);

/**
 * @fn int VD_Get_WWN(IN int enc_id, IN int port_id, OUT char* wwn, IN unsigned int buf_len)
 * @brief Retrieve the WWN for the specified virtual disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the WWN of the specified virtual disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Virtual disk port id assigned by system dependent layer. 
 * @param[out] wwn WWN returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completed.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the 
 *         internal configurationturn file.
 */ 
int VD_Get_WWN(IN int enc_id, IN int port_id, OUT char* wwn, IN unsigned int buf_len);

/**
 * @fn int PD_Get_WWN_By_Dev_No(IN dev_t dev_no, OUT char* wwn, IN unsigned int buf_len)
 * @brief Retrieve the WWN for the specified device number of physical disk.
 *
 * This function is called by boot utility to obtain the WWN of the specified physical disk.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @param[out] wwn WWN returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completed.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_WWN_By_Dev_No(IN dev_t dev_no, OUT char* wwn, IN unsigned int buf_len);

/**
 * @fn int int PD_Is_In_Root(IN int enc_id, IN int port_id)
 * @brief Check if the specified physical disk is placed in root enclosure (NAS).
 *
 * This function is called by nas utility to check if the PD has been placed in root enclosure.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 TRUE, the specified pd is placed in root enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root enclosure.
 */
int PD_Is_In_Root(IN int enc_id, IN int port_id);

/**
 * @fn int int PD_Is_In_Root_Cache(IN int enc_id, IN int port_id)
 * @brief Check if the specified physical disk is placed in root cache enclosure.
 *
 * This function is called by nas utility to check if the PD has been placed in root cache enclosure.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 TRUE, the specified pd is placed in root cache enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root cache enclosure.
 */
int PD_Is_In_Root_Cache(IN int enc_id, IN int port_id);

/**
 * @fn int int PD_Is_In_Root_PCIe(IN int enc_id, IN int port_id)
 * @brief Check if the specified physical disk is placed in root PCIe enclosure.
 *
 * This function is called by nas utility to check if the PD has been placed in root PCIe enclosure.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 TRUE, the specified pd is placed in root PCIe enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root PCIe enclosure.
 */
int PD_Is_In_Root_PCIe(IN int enc_id, IN int port_id);

/**
 * @fn int int PD_Is_Report_Healthy(IN int sense_code)
 * @brief Check if the specified physical disk is healthy
 *
 * This function is called by nas utility to check if the PD is healthy or not.
 * @param[in] sense_code Physical disk return sense code.
 * @retval 1 TRUE, the specified pd is healthy.
 * @retval 0 FALSE, the specified pd is not healthy.
 */
int PD_Is_Report_Healthy(IN int sense_code);

/**
 * @fn int PD_Is_In_Front_Of_Root_USB(IN int enc_id, IN int port_id)
 * @brief Check if the specified physical disk is placed in the front usb port of the root enclosure (NAS).
 *
 * This function is called by nas utility to check if the PD has been placed in the front usb port.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 TRUE, the specified pd is placed in the front usb port.
 * @retval 0 FALSE, the specified pd is not placed in the front usb port.
 */
int PD_Is_In_Front_Of_Root_USB(IN int enc_id, IN int port_id);

/**
 * @fn int int PD_Is_In_Root_By_Dev_No(IN dev_t dev_no)
 * @brief Check if the specified physical disk is placed in root enclosure (NAS).
 *
 * This function is called by boot utility to check if the PD has been placed in root enclosure.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @retval 1 TRUE, the specified pd is placed in root enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root enclosure.
 */
int PD_Is_In_Root_By_Dev_No(IN dev_t dev_no);

/**
 * @fn int int PD_Is_In_Root_Cache_By_Dev_No(IN dev_t dev_no)
 * @brief Check if the specified physical disk is placed in root cache enclosure.
 *
 * This function is called by boot utility to check if the PD has been placed in root cache enclosure.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @retval 1 TRUE, the specified pd is placed in root cache enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root cache enclosure.
 */
int PD_Is_In_Root_Cache_By_Dev_No(IN dev_t dev_no);

/**
 * @fn int int PD_Is_In_Root_PCIe_By_Dev_No(IN dev_t dev_no)
 * @brief Check if the specified physical disk is placed in root PCIe enclosure.
 *
 * This function is called by boot utility to check if the PD has been placed in root PCIe enclosure.
 * @param[in] dev_no The device number (major, minor) of the block special file.
 * @retval 1 TRUE, the specified pd is placed in root PCIe enclosure.
 * @retval 0 FALSE, the specified pd is not placed in root PCIe enclosure.
 */
int PD_Is_In_Root_PCIe_By_Dev_No(IN dev_t dev_no);

/**
 * @fn int PD_Get_Sys_Name_By_WWN(IN char* wwn, OUT char* sys_name, IN unsigned int buf_len);
 * @brief Retrieve the system name according to the specified world wide name of physical disk.
 *
 * This function is called by system utility to obtain the system dependent name according to the wwn of physical disk.
 * @param[in] wwn World wide name of the physical disk.
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Sys_Name_By_WWN(IN char* wwn, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Ctrl_Name(IN int enc_id, IN int port_id, OUT char* ctrl_name, IN unsigned int buf_len)
 * @brief Retrieve the control name for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the control name for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] ctrl_name System dependent control name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the ctrl_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the control name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Ctrl_Name(IN int enc_id, IN int port_id, OUT char* ctrl_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Bus_Name(IN int enc_id, IN int port_id, OUT char* bus_name, IN unsigned int buf_len)
 * @brief Retrieve the bus name for specified physical disk from the internal configuration file.
 *
 * This function is called by system utility to obtain the bus name for specified physical disk.
 * @param[in] enc_id Logical enclosure identifier (0 based).
 * @param[in] port_id Physical port id assigned by system dependent layer (1 based).
 * @param[out] bus_name System dependent bus name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the ctrl_name.
 * @retval 0 Success, the specified physical disk is retrieved completed.
 * @retval -1 Generic failure, Fail to retrieve the control name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Bus_Name(IN int enc_id, IN int port_id, OUT char* bus_name, IN unsigned int buf_len);

/**
 * @fn int PD_Get_Ctrl_Name_By_WWN(IN char* wwn, OUT char* ctrl_name, IN unsigned int buf_len);
 * @brief Retrieve the control name according to the specified world wide name of physical disk.
 *
 * This function is called by system utility to obtain the system dependent control name according to the wwn of physical disk.
 * @param[in] wwn World wide name of the physical disk.
 * @param[out] ctrl_name System dependent control name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified physical disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified physical disk in the
 *         internal configurationturn file.
 */
int PD_Get_Ctrl_Name_By_WWN(IN char* wwn, OUT char* ctrl_name, IN unsigned int buf_len);

/**
 * int PD_Get_Phy_Id_By_Ctrl_Name(IN char* ctrl_name, IN uint32_t ctrl_len,
 *   OUT int32_t *att_phy_id)
 * @brief get the specific port attached to which phy id from sysfs.
 *
 * This function will get the specific port attahced to which phy id from sysfs.
 * @param[in] ctrl_name Disk control name (ex. sg1).
 * @param[in] ctrl_len Lenth of ctrl_name.
 * @param[out] att_phy_id Attahced phy id. if ata, this field will be -1.
 * @retval 0  Succes.
 * @retval -1 Fail to parse sysfs or the drive attached behind ata adapter.
 */
int PD_Get_Att_Phy_Id_By_Ctrl_Name(IN char* ctrl_name,
    IN uint32_t ctrl_len, OUT int32_t *att_phy_id);

/**
 * @fn int PD_Get_Capacity(IN int enc_id, IN int port_id,
 *                         OUT unsigned long long* capacityP, OUT unsigned int* sector_sizeP)
 * @brief Retrieve the specified physical disk capacity.
 *
 * This function will retrieve the capacity and sector size for the specified physical disk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] capacityP The physical disk capacity in terms of sector.
 * @param[out] sector_sizeP The sector size of the specified physical disk.
 * @retval 0 Success, the specified physical disk capacity is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Capacity(IN int enc_id, IN int port_id,
                    OUT unsigned long long* capacityP, OUT unsigned int* sector_sizeP);

/**
 * @fn int PD_LINK_SPEED PD_Get_Link_Speed(IN int enc_id, IN int port_id)
 * @brief Retrieve the link speed of the specified physical disk.
 *
 * This function will retrieve the link speed for the specified physical disk.
 * Currently only USB physical disk is supported.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval The link speed of the specified physical disk.
 */
PD_LINK_SPEED PD_Get_Link_Speed(IN int enc_id, IN int port_id);

/**
 * @fn int PD_LINK_SPEED PD_Get_PD_Max_Link_Speed(IN int enc_id, IN int port_id)
 * @brief Retrieve the PD max link speed of the specified physical disk.
 *
 * This function will retrieve the PD max link speed for the specified physical disk.
 * Currently only USB physical disk is supported.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval The max link speed of the specified physical disk.
 */
PD_LINK_SPEED PD_Get_PD_Max_Link_Speed(IN int enc_id, IN int port_id);

#ifdef STORAGE_V2

// KS-Redmine#28626.
/**
 * @fn int PD_Get_Read_Speed(IN int enc_id, IN int port_id, OUT unsigned long long* speedP)
 * @brief Retrieve the specified physical disk read speed in KB.
 *
 * This function will retrieve the specified physical disk read speed in KB.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] speedP The physical disk read speed in KB.
 * @retval 0 Success, the specified physical disk read speed is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Read_Speed(IN int enc_id, IN int port_id, OUT unsigned long long* speedP);

/**
 * @fn int PD_Get_Read_IOPS(IN int enc_id, IN int port_id, OUT unsigned long long* iopsP)
 * @brief Retrieve the specified physical disk read iops.
 *
 * This function will retrieve the specified physical disk read iops.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] iops The physical disk read iops.
 * @retval 0 Success, the specified physical disk read iops is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Read_IOPS(IN int enc_id, IN int port_id, OUT unsigned long long* iopsP);

// KS-Redmine#29829.
/**
 * @fn int PD_Get_Write_Speed(IN int enc_id, IN int port_id, OUT unsigned long long* speedP)
 * @brief Retrieve the specified physical disk write speed in Byte.
 *
 * This function will retrieve the specified physical disk write speed in Byte.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] speedP The physical disk write speed in Byte.
 * @retval 0 Success, the specified physical disk write speed is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Write_Speed(IN int enc_id, IN int port_id, OUT unsigned long long* speedP);

/**
 * @fn int PD_Get_Write_IOPS(IN int enc_id, IN int port_id, OUT unsigned long long* iopsP)
 * @brief Retrieve the specified physical disk write iops.
 *
 * This function will retrieve the specified physical disk write iops.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] iops The physical disk write iops.
 * @retval 0 Success, the specified physical disk write iops is retrieved completely.
 * @retval -1 Generic failure.
 */
int PD_Get_Write_IOPS(IN int enc_id, IN int port_id, OUT unsigned long long* iopsP);

/**
 * @fn int PD_LINK_SPEED PD_Get_Max_Link_Speed(IN int enc_id, IN int port_id)
 * @brief Retrieve the max link speed of the Host interface.
 *
 * This function will retrieve the max link speed for the Host port interface.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval The max link speed of the Host port interface.
 */
PD_LINK_SPEED PD_Get_Max_Link_Speed(IN int enc_id, IN int port_id);

/**
 * @fn int PD_set_checking_read_speed(IN int enc_id, IN int port_id, IN int enable_weekly_test)
 * @brief Test reading speed of all disks.
 *
 * This function will retrieve the max link speed for the specified physical disk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
  */
int PD_set_checking_read_speed(IN int enc_id, IN int port_id);

// KS-Redmine#39850.
#if 0
// The following functions may be replaced by HAL API.
/**
 * @fn int PD_SED_Is_Valid(IN int enc_id, IN int port_id)
 * @brief Verify whether the given device is SED or not.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval 1: SED device; 0: Not SED device.
 */
int PD_SED_Is_Valid(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SED_Is_Lock(IN int enc_id, IN int port_id)
 * @brief Check the SED is locked or not.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval 1: SED device and locked; 0: Not SED device or not locked.
 */
int PD_SED_Is_Lock(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SED_Initial_Setup(IN int enc_id, IN int port_id, IN char* password)
 * @brief Setup the device for SED use.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] password Password.
 * @retval 0 Success.
 * @retval < 0 Failure.
 */
int PD_SED_Initial_Setup(IN int enc_id, IN int port_id, IN char* password);

/**
 * @fn int PD_SED_Revert(IN int enc_id, IN int port_id, IN char* password)
 * @brief Deactivate the Locking SP.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] password Password.
 * @retval 0 Success.
 * @retval < 0 Failure.
 */
int PD_SED_Revert(IN int enc_id, IN int port_id, IN char* password);

/**
 * @fn int PD_SED_Set_Passwd(IN int enc_id, IN int port_id, IN char* old_password, IN char* new_password)
 * @brief Change the SID and Admin1 password.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] old_password Old password.
 * @param[in] new_password New password.
 * @retval 0 Success.
 * @retval < 0 Failure.
 */
int PD_SED_Set_Passwd(IN int enc_id, IN int port_id, IN char* old_password, IN char* new_password);

/**
 * @fn int PD_SED_Verify_Passwd(IN int enc_id, IN int port_id, IN char* password)
 * @brief Verify the password.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] password Password.
 * @retval 0 Success.
 * @retval < 0 Failure.
 */
int PD_SED_Verify_Passwd(IN int enc_id, IN int port_id, IN char* password);
#endif
#endif

/**
 * @fn int PD_Get_Size_Str(IN int enc_id, IN int port_id,
 *                         IN SIZE_UNIT unit, IN OUT char* size_str, IN unsigned int buf_len)
 * @brief Convert the physical disk capacity according to the specified size unit.
 *
 * This function will caculate the physical disk capacity based on the specified size unit.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] unit The conversion unit.
 * @param[out] size_str The converted output string will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval size_str Return the converted output string.
 */
char* PD_Get_Size_Str(IN int enc_id, IN int port_id,
                      IN SIZE_UNIT unit, IN OUT char* size_str, IN unsigned int buf_len);



/**
 * @fn int PD_Get_SCSI_Log_Pages(IN int enc_id, IN int port_id, OUT int* sense_code,
 *                               OUT PD_SCSI_LOG_PAGE log_page_ary[], IN int page_ary_count);
 * @brief Enumerate available log sense pages for specified SCSI/SAS physical disk.
 *
 * This function is called by nas utility to obtain all available SCSI/SAS log sense pages.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] sense_code request sense return status.
 * @param[out] log_page_ary A log sense page array to host the available SCSI/SAS log sense page.
 * @param[in] page_ary_count The entry count of the page array.
 * @retval >=0 Success, return the count of the available SCSI/SAS log sense pages, if the count is larger than
 *         page_ary_count, the calling utility should allocate more space for page array to
 *         obtain all available pages.
 * @retval Generic failure.
 */
int PD_Get_SCSI_Log_Pages(IN int enc_id, IN int port_id, OUT int* sense_code,
                          OUT PD_SCSI_LOG_PAGE log_page_ary[], IN int page_ary_count);

/**
 * @fn int PD_SMART_Get_NVME_Info(IN int enc_id, IN int port_id, OUT PD_NVME_SMART_LOG_PAGE* smart_log)
 * @brief Get available smart info for specified NVME physical disk.
 *
 * This function is called by nas utility to obtain available smart info for NVME drive.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] smart_log request smart info.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_SMART_Get_NVME_Info(IN int enc_id, IN int port_id, OUT PD_NVME_SMART_LOG_PAGE* smart_log);

/**
 * @fn int PD_Set_Self_Test_Mode(IN int enc_id, IN int port_id, IN PD_SELFTEST_MODE test_mode)
 * @brief Request the specified HDD to perform self-test operation.
 *
 * This function requests the HDD to perform the corresponding self-test (diagnostic) operations.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] test_mode The HDD test mode that is request to proceed.
 * @retval 0 Success, the self-test is proceeding.
 * @retval -1 Generic failure.
 */
int PD_Set_Self_Test_Mode(IN int enc_id, IN int port_id, IN PD_SELFTEST_MODE test_mode);

/**
 * @fn int PD_Set_Error_Recovery_Timer(IN int enc_id, IN int port_id, IN int write_timer, IN int val_100ms)
 * @brief Set the specified HDD write/read error recovery time.
 *
 * This function requests the HDD to set the corresponding write/read error recovery time.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[in] write_timer Speficeid write/read timer.
 * @param[in] val_100ms New timer value ,100ms units.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Set_Error_Recovery_Timer(IN int enc_id, IN int port_id, IN int write_timer, IN int val_100ms);

/**
 * @fn int PD_Get_Error_Recovery_Timer(IN int enc_id, IN int port_id, IN int write_timer, OUT int* val_100ms)
 * @brief Get the specified HDD write/read error recovery time.
 *
 * This function requests the HDD to get the corresponding write/read error recovery time.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[in] write_timer Speficeid write/read timer.
 * @param[out] val_100ms Return current timer value,100ms units.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Get_Error_Recovery_Timer(IN int enc_id, IN int port_id, IN int write_timer, OUT int* val_100ms);

/**
 * @fn int PD_Get_SCSI_Self_Test_Status(IN int enc_id, IN int port_id,
 *                                      OUT PD_SCSI_SELFTEST_STATUS* test_statusP, OUT int* test_percentageP);
 * @brief Retrieve the SCSI HDD self-test status.
 *
 * This function is invoked to retrieve the current self-test status.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] test_statusP The self test status if it is proceeding.
 * @param[out] test_percentageP The percent of the self-test routine remaining until completion in ten percent increments.
 * @retval 0 Success, the self-test is proceeding or completed.
 * @retval -1 Generic failure.
 */
int PD_Get_SCSI_Self_Test_Status(IN int enc_id, IN int port_id,
                                 OUT PD_SCSI_SELFTEST_STATUS* test_statusP, OUT int* test_percentageP);

/**
 * @fn int PD_Get_Power_Status(IN int enc_id, IN int port_id, OUT PD_POWER_STATUS *statusP)
 * @brief Retrieve physical disk power status.
 *
 * This function is called to obtain the power status of the specified physical disk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] statusP The power status of the specified physical disk.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Get_Power_Status(IN int enc_id, IN int port_id, OUT PD_POWER_STATUS *statusP);

/**
 * @fn int PD_Set_Power_Standby(IN int enc_id, IN int port_id)
 * @brief Change the physical disk power status.
 *
 * This function is called to change the power status of the specified physical disk to PD_STATUS_STANDBY.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Set_Power_Standby(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Get_Phy_Err_Cnt(IN int enc_id, IN int port_id, IN FILE *fd,
 *     OUT PHY_ERR_CNT_INFO *err_cnt, OUT PD_TYPE *pd_type)
 * @brief Get phy error counts of particular disk.
 *
 * This function will get phy error counts of particular disk.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[in] fd Stream to redirect debug stream.
 * @param[out] err_cnt Phy error counts, please refer pd_type to access.
 * @param[out] pd_type PD type.
 * @retval 0 Success.
 * @retval -1 Error.
 */
int PD_Get_Phy_Err_Cnt(IN int enc_id, IN int port_id, IN FILE *fd,
    OUT PHY_ERR_CNT_INFO *err_cnt, OUT PD_TYPE *pd_type);

/**
 * @fn int PD_Get_Life_Used_Percentage(IN int enc_id, IN int port_id, OUT int *percentageP)
 * @brief Retrieve the life used percentage of the specified physical disk.
 *
 * This function is called to obtain the life used percentage of the specified physical disk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] percentageP The life used percentage of the specified physical disk.
 *             According to the spec, the percentage value is allowed to exceed 100.
 *             A value of 100 indicates that the estimated endurance of the device has been consumed
 *             , but may not indicate a device failure.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Get_Life_Used_Percentage(IN int enc_id, IN int port_id, OUT int *percentageP);

/**
 * @fn int PD_Check_Attach_To_SAS_HBA_Directly(IN int enc_id, IN int port_id)
 * @brief Check specific PD whether attaching to sas hba directly.
 *
 * This function will check specific pd whether attaching to sas hba directly.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @retval 0 Somethink wrong or not attached to sas hba directly.
 * @retval 1 PD attached behind sas hba directly.
 */
int PD_Check_Attach_To_SAS_HBA_Directly(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Get_Log_Name(IN int enc_id, IN int port_id, OUT char* buf, IN int buf_len)
 * @brief Get the disk name for system event log
 *
 * This function is used when output disk name in system event log
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[out] buf Buffer to store the disk name.
 * @param[in] buf_len Buffer length.
 * @retval 0 Get Log name successfully.
 * @retval 1 Get Slot Log name successfully, 
 * @retval -1 General error. 
 */
 
int PD_Get_Log_Name(IN int enc_id, IN int port_id, OUT char *buf, IN int buf_len);

/**
 * @fn int PD_Get_Dev_Slot_Name(IN int enc_id, IN int port_id, OUT char* buf, IN int buf_len)
 * @brief Get the disk name for system event log
 *
 * This function is used when output disk name in system event log
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[out] buf Buffer to store the disk name.
 * @param[in] buf_len Buffer length.
 * @retval 0 Get Log name successfully.
 * @retval 1 Get Slot Log name successfully, 
 * @retval -1 General error. 
 */
int PD_Get_Dev_Slot_Name(IN int enc_id, IN int port_id, OUT char *buf, IN int buf_len);

// bug 48045 NCQ enable/disable scheme
/**
 * @fn int PD_ENCQ_Is_Activated(IN int enc_id, IN int port_id)
 * @brief Check the ENCQ scheme is activated.
 *
 * This function is used to check the ENCQ scheme is activated or not.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 ENCQ Scheme is activated on this disk.
 * @retval 1 ENCQ Scheme not activated on this disk.
 * @retval -1 General error.
 */
int PD_ENCQ_Is_Activated(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ENCQ_Get_Info(IN int enc_id, IN int port_id)
 * @brief Get the disk NCQ info.
 *
 * This function is used to get the disk NCQ info .
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk NCQ off.
 * @retval 1 Disk NCQ on.
 * @retval -1 Disk no support NCQ.
 * @retval -2 General error.
 */
int PD_ENCQ_Get_Info(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Is_Support_NCQ(IN int enc_id, IN int port_id)
 * @brief Check the disk ncq support.
 *
 * This function is used to check disk ncq support.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk NCQ not support.
 * @retval 1 Disk NCQ support.
 * @retval -1 General error.
 */
int PD_Is_Support_NCQ(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Reconfig_Queue_Depth(IN int enc_id, IN int port_id)
 * @brief Check the disk whether needing to reconfig queue depth.
 *
 * This function will reconfig queue depth if necessary.
 *
 * @param[in] enc_id Enclosure identifier.
 * @param[in] pd_id Physical disk identifier.
 *
 * @retval 0 Disk not needs reconfig queue depth.
 * @retval >0 Newer queue depth that has been config.
 * @retval -1 General error.
 */
int PD_Reconfig_Queue_Depth(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Is_Attached_Behind_SAS(IN int enc_id, IN int port_id)
 *
 * @brief Check specific pd bus whether attaching behind SAS controller.
 *
 * This function will check pd bus whether had a dir in sas_host.
 *
 * @param[in] pd_bus_name PD bus name.
 * @retval 1 In SAS domain.
 * @retval 0 Is not in SAS domain or something error.
 */
int PD_Is_Attached_Behind_SAS(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ENCQ_Set_Info(IN int enc_id, IN int port_id, IN int on_off, IN int boot)
 * @brief Enable/disable disk ncq.
 *
 * This function is used to enable/disable disk ncq.
 * If boot is not setted, it will set ENCQ configure and
 * write system log.
 * If disk model is in blacklist, it will always disable ncq.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] on_off Enable or disable flag.
 * @param[in] boot  Is called from booting hal_util.
 * @retval 0 Success.
 * @retval -1 General error.
 */
int PD_ENCQ_Set_Info(IN int enc_id, IN int port_id, IN int on_off, IN int boot);

/**
 * @fn int PD_ENCQ_Is_Blacklisted(IN int enc_id, IN int port_id)
 * @brief Check the disk model is in ENCQ black list
 *
 * This function is used to check the disk model in ENCQ black list or not
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk model not in black list.
 * @retval 1 Disk model in black list.
 * @retval -1 General error.
 */
int PD_ENCQ_Is_Blacklisted(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ENCQ_Get_Default_Config(IN int enc_id, IN int port_id)
 * @brief Get the disk ENCQ default configure.
 *
 * This function is used to get disk ENCQ default configure.
 * Only called when booting and hot-plug indirectly.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk NCQ default setting is off.
 * @retval 1 Disk NCQ default setting is on.
 * @retval -1 General error.
 */
int PD_ENCQ_Get_Default_Config(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ENCQ_Get_Config(IN int enc_id, IN int port_id)
 * @brief Get the disk ENCQ setting
 *
 * This function is used to get disk ENCQ setting.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk NCQ setting is off.
 * @retval 1 Disk NCQ setting is on.
 * @retval -1 General error.
 */
int PD_ENCQ_Get_Config(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ENCQ_Set_Config(IN int enc_id, IN int port_id, IN int on_off)
 * @brief Set the disk ENCQ configure.
 *
 * This function is used to get disk ENCQ configure.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] on_off Enable/Disable flags
 * @retval 0 Success.
 * @retval -1 General error.
 */
int PD_ENCQ_Set_Config(IN int enc_id, IN int port_id, IN int on_off);

/**
 * @fn int PD_Get_Env_Sensor_Temp(IN int enc_id, IN int port_id, OUT int *tempP)
 * @brief Retrieve environment sensor temperature of the specified physical disk.
 *
 * This function is called by nas utility to obtain the environment sensor temperature of the specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] tempP a pointer of variable to store the returned temperature value.
 * @retval 0 Success, the specified sensor temperature is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified sensor temperature.
 */
int PD_Get_Env_Sensor_Temp(IN int enc_id, IN int port_id, OUT int *tempP);

/**
 * @fn void PD_Remove_and_Backup_Section(IN int enc_id, IN int port_id)
 * @brief Remove the specified PD section from the enclosure config file
 *
 * This function is used to remove the specified PD section from the enclosure config file
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Success, the specified PD section is removed.
 * @retval -1 Generic failure, Fail to remove the specified PD section.
 */
void PD_Remove_and_Backup_Section(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Is_WWN_Blacklisted(IN int enc_id, IN int port_id)
 * @brief Check if the wwn of the specified port is in the QNAP WWN blacklist.
 *
 * This function will check if the wwn of the specified port is in the QNAP WWN blacklist.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 The wwn of the specified port is no in the blacklist
 * @retval 1 The wwn of the specified port is no in the blacklist
 */
int PD_Is_WWN_Blacklisted(IN int enc_id, IN int port_id);

#if 1	//SED Jimmy 20181123

/**
 * @fn int PD_SED_Is_Supported(IN int enc_id, IN int port_id)
 * @brief Check if the disk supports SED feature.
 *
 * This function is used to check if the disk supports SED feature or not.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk does support SED feature. 
 * @retval 1 Disk does NOT support SED support. 
 * @retval -1 General error. 
 */
int PD_SED_Is_Supported(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SED_Is_Initialized(IN int enc_id, IN int port_id)
 * @brief Check if the disk SED feature is already initialized.
 *
 * This function is used to check if the disk SED feature is already initialized,
 * which means the password has already been set.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 Disk SED feature is NOT initialized. 
 * @retval 1 Disk SED feature is initialized. 
 * @retval -1 General error. 
 */
int PD_SED_Is_Initialized(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SED_Initialize(IN int enc_id, IN int port_id, IN char* passwd)
 * @brief Initialize the specified SED drive.
 *
 * This function is used to initialize the SED drive with the specified password. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd SID/Admin1 password.
 * @retval 0 Initialize the SED drive successfully.
 * @retval -1 General error.
 */
int PD_SED_Initialize(IN int enc_id, IN int port_id, IN char *passwd);

/**
 * @fn int PD_SED_MBR_Enable(IN int enc_id, IN int port_id, IN char passwd, IN int enable) 
 * @brief Enable or disable MBR shadowing of the specified SED drive. 
 * 
 * This function is used to enable or disable MBR shadowing of the specified SED drive.  
 * @param[in] enc_id Enclosure identifier. * @param[in] port_id Physical disk identifier. 
 * @param[in] passwd SID/Admin1 password. * @param[in] enable 1: enable 0: disable 
 * @retval 0 Enable or disable MBR shadowing of the SED drive successfully. 
 * @retval -1 General error. 
 */
int PD_SED_MBR_Enable(IN int enc_id, IN int port_id, IN char *passwd, IN int enable);

/**
 * @fn int PD_SED_Display_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id, OUT int *start_sector, OUT *int size_sector)
 * @brief Display the LBA range setting of the specified locking range id.
 *
 * This function is used to display the LBA range setting of the specified locking range id.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could NOT be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be displayed.
 * @param[out] start_sector The start LBA of the specified locking range.
 * @param[out] size_sector The size in sector of the specified locking range.
 * @retval 0 Display the specified locking range successfully.
 * @retval -1 General error.
 * @retval -2 Range id 0 is specified.
 */
int PD_SED_Display_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id, OUT int *start_sector, OUT int *size_sector);

/**
 * @fn int PD_SED_Setup_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id, IN int start_sector, IN int size_sector)
 * @brief Setup the LBA range of the specified locking range id.
 *
 * This function is used to setup the LBA range of the specified locking range id.
 * Note that after the locking range is setup, all the data within that range would be cleared.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could NOT be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be setup. 
 * @param[in] start_sector The start LBA of the specified locking range. Should be 4KB aligned.
 * @param[in] size_sector The size in sector of the specified locking range. Should be 4KB aligned.
 * @retval 0 Setup the specified locking range successfully.
 * @retval -1 General error.
 * @retval -2 Range id 0 is specified.
 */
int PD_SED_Setup_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id, IN int start_sector, IN int size_sector);

/**
 * @fn int PD_SED_Remove_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Remove the LBA range setting of the specified locking range id.
 *
 * This function is used to remove the LBA range setting of the specified locking range id.
 * Note that after the locking range setting is removed, all the data within that range would be cleared.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could NOT be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be removed. 
 * @retval 0 Remove the specified locking range successfully.
 * @retval -1 General error.
 * @retval -2 Range id 0 is specified.
 */
int PD_SED_Remove_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Range_Is_Enabled(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Check if the specified locking range is enabled.
 *
 * This function is used to check if the specified locking range is enabled. 
 * The locking range is enabled only means the ResetOnLock is enabled, and the range would be automatically locked 
 * after power cycle. It does NOT mean the range is locked at this moment. 
 * To know the lock status, use PD_SED_Range_Is_Locked() instead.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be queried.
 * @retval 0 The specified locking range is disabled.
 * @retval 1 The specified locking range is enabled.
 * @retval -1 General error.
 */
int PD_SED_Range_Is_Enabled(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Is_Locked(IN int enc_id, IN int port_id)
 * @brief Check if the SED is locked.
 *
 * This function is used to check if the SED is locked or not. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 0 The SED is unlocked.
 * @retval 1 The SED is locked.
 * @retval -1 General error.
 */
int PD_SED_Is_Locked(IN int enc_id,IN int port_id);

/**
 * @fn int PD_SED_Enable_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Enable the specified locking range.
 *
 * This function is used to enable the specified locking range. 
 * Enable the locking range only means the ResetOnLock is enabled, the range would be automatically locked 
 * after power cycle. It does NOT mean the range is locked after invoking this API.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be enabled.
 * @retval 0 Enable the specified locking range successfully.
 * @retval -1 General error.
 */
int PD_SED_Enable_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Disable_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Disable the specified locking range.
 *
 * This function is used to disable the specified locking range. 
 * Disable the locking range only means the ResetOnLock is disabled, the range would NOT be automatically locked 
 * after power cycle. It does NOT mean the range is unlocked after invoking this API.
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be disabled.
 * @retval 0 Disable the specified locking range successfully.
 * @retval -1 General error.
 */
int PD_SED_Disable_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Range_Is_Locked(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Check if the specified locking range is locked.
 *
 * This function is used to check if the specified locking range is locked or not. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be queried.
 * @retval 0 The specified locking range is unlocked.
 * @retval 1 The specified locking range is locked.
 * @retval -1 General error.
 */
int PD_SED_Range_Is_Locked(IN int enc_id, IN int port_id, IN char *passwd, IN int range_idx);

/**
 * @fn int PD_SED_Lock_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Lock the specified locking range.
 *
 * This function is used to lock the specified locking range. 
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be locked.
 * @retval 0 Lock the specified locking range successfully.
 * @retval -1 General error.
 */
int PD_SED_Lock_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Unlock_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id)
 * @brief Unlock the specified locking range.
 *
 * This function is used to unlock the specified locking range. 
 * Range id 0 is a global locking range and it represents the whole disk. Range id 0 could be used for this API. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password.
 * @param[in] range_id The locking range id to be unlocked.
 * @retval 0 Unlock the specified locking range successfully.
 * @retval -1 General error.
 */
int PD_SED_Unlock_Range(IN int enc_id, IN int port_id, IN char *passwd, IN int range_id);

/**
 * @fn int PD_SED_Change_Password(IN int enc_id, IN int port_id, IN char* old_passwd, IN char* new_passwd)
 * @brief Change the password of the specified drive.
 *
 * This function is used to change the password of the specified SED drive. 
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] old_passwd old password.
 * @param[in] new_passwd new password.
 * @retval 0 Change the password successfully.
 * @retval -1 General error.
 */
int PD_SED_Change_Password(IN int enc_id, IN int port_id, IN char *old_passwd, IN char *new_passwd);

/**
 * @fn int PD_SED_Verify_Password(IN int enc_id, IN int port_id, IN char* password)
 * @brief Verify the password.
 *
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] password Password.
 * @retval 0 Success.
 * @retval < 0 Failure.
 */

int PD_SED_Verify_Password(IN int enc_id, IN int port_id, IN char *passwd);

/**
 * @fn int PD_SED_Factory_Reset_With_Password(IN int enc_id, IN int port_id, IN char* passwd)
 * @brief Reset a SED drive to factory default state by the Admin1 password.
 *
 * This function is used to restore the SED drive to factory default state by the Admin1 password, 
 * all data will be erase and the password will be cleared.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] passwd Admin1 password of this drive.
 * @retval 0 Reset the specified SED drive successfully.
 * @retval -1 General error.
 */
int PD_SED_Factory_Reset_With_Password(IN int enc_id, IN int port_id, IN char* passwd);

/**
 * @fn int PD_SED_Factory_Reset_With_PSID(IN int enc_id, IN int port_id, IN char* PSID)
 * @brief Reset a SED drive to factory default state by the PSID.
 *
 * This function is used to restore the SED drive to factory default state by the PSID, 
 * all data will be erase and the password will be cleared.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @param[in] PSID PSID of this drive.
 * @retval 0 Reset the specified SED drive successfully.
 * @retval -1 General error.
 */
int PD_SED_Factory_Reset_With_PSID(IN int enc_id, IN int port_id, IN char *PSID);

#endif
/*@}*/  /* end of physical disk module */

/*@}*/    /* begin of SR model Function */
typedef enum _SR_DEV_TYPE
{
    SR_UNKNOWN  = -1,           /*!< Unknown SR device (-1) */
    SR_A2AR      = 0,            /*!< SR-A2AR  (0) */
    SR_A2MAR     = 1,            /*!< SR-A2MAR (1) */
} SR_DEV_TYPE;

typedef enum _SR_RAID_LEVEL
{
    SR_RAID0    = 0,            /*!< RAID 0 (0) */
    SR_RAID1    = 1,            /*!< RAID 1 (1) */
    SR_JBOD     = 2,            /*!< JBOD   (2) */
} SR_RAID_LEVEL;

typedef enum _SR_SWITCH_MODE
{
    SR_SWITCH_JBOD      = 0,            /*!< JBOD                       */
    SR_SWITCH_RAID1     = 1,            /*!< RAID1                      */
    SR_SWITCH_RAID0     = 2,            /*!< RAID0                      */
    SR_SWITCH_PM        = 3,            /*!< PM mode (Individual Mode)  */
    SR_SWITCH_UNKNOWN   = 0xff,         /*!< Unknown Mode               */
} SR_SWITCH_MODE;

typedef enum _SR_RAID_STATUS
{
    SR_RAID_BROKEN      = 0,
    SR_RAID_DEGRADED    = 1,
    SR_RAID_REBUILD     = 2,
    SR_RAID_NORMAL      = 3,
} SR_RAID_STATUS;

typedef enum _SR_RAID_REBUILD_PRIORITY
{
    SR_REBUILD_PRIO_LOWEST      = 1,
    SR_REBUILD_PRIO_LOW         = 2,
    SR_REBUILD_PRIO_MEDIAN      = 3,
    SR_REBUILD_PRIO_HIGH        = 4,
    SR_REBUILD_PRIO_HIGHEST     = 5,
} SR_RAID_REBUILD_PRIORITY;

typedef enum _SR_EVENT
{
    SR_EVENT_RAID_FW_UPDATE_START         = 0,
    SR_EVENT_RAID_FW_UPDATE_FINISH_OLD_NEW,
    SR_EVENT_RAID_FW_UPDATE_FINISH,
    SR_EVENT_RAID_FW_UPDATE_FAIL_OLD_NEW,
    SR_EVENT_RAID_FW_UPDATE_FAIL,
    SR_EVENT_RAID_DEGRADED_CHK_STATUS     = 10,
    SR_EVENT_RAID_DEGRADED_REBUILD,
    SR_EVENT_RAID_FAIL,
    SR_EVENT_RAID_REBUILD_START           = 20,
    SR_EVENT_RAID_REBUILD_FINISH,
    SR_EVENT_RAID_REBUILD_FAIL,
} SR_EVENT;

/**
 * @struct     SR_RAID_INFO
 * @brief      Disk array runtime statistics information.
 */
typedef struct _SR_RAID_INFO
{
    SR_RAID_LEVEL raid_level;                   /*!< Disk array level */
    int raid_dev_num;                           /*!< The number of data drives */
    SR_RAID_STATUS status;                      /*!< The status of disk array */
    SR_RAID_REBUILD_PRIORITY prio;              /*!< The rebuild priority of disk array */
    unsigned long long array_size;              /*!< Disk array total capacity, in KB */
    float rebuild_percent;                      /*!< The rebuild percentage of disk array */
} SR_RAID_INFO;

/**
 * @fn int SR_RAID_Adjust_Rebuild_Priority(IN int raid_id, IN SR_RAID_REBUILD_PRIORITY prio)
 * @brief Set the rebuild priority of the h/w raid with the specified raid_id.
 *
 * This function sets the rebuild priority of the h/w raid with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[in] prio The rebuild priorty, which is [1..5], but only [2..4] is acceptible by the uppder layer. level-1 is the same as level-2; level-5 is the same as level-4
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int SR_RAID_Adjust_Rebuild_Priority(IN int raid_id, IN SR_RAID_REBUILD_PRIORITY prio);

/**
 * @fn int SR_RAID_Get_Info(IN int raid_id, OUT SR_RAID_INFO* SR_raid_infoP)
 * @brief Retrieve the h/w raid status information with the specified raid_id.
 *
 * This function retrieves the h/w raid status information with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[out] SR_raid_infoP The structure used to store the h/w raid status and information.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int SR_RAID_Get_Info(IN int raid_id, OUT SR_RAID_INFO* SR_raid_infoP);

/**
 * @fn int PD_Get_SR_Mode(IN int enc_id, IN int port_id)
 * @brief Get SR mode from enclosure_0.conf
 *
 * This function is called to get SR mode.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk identifier.
 * @retval 9 PD_NON_SR_MODE
 * @retval others, PD_SR_MODE_PM = 0, PD_SR_MODE_RAID0 = 1, PD_SR_MODE_RAID1 = 2, PD_SR_MODE_JBOD = 3
 */
int PD_Get_SR_Mode(IN int enc_id, IN int port_id);

/**
 * @fn int SR_Get_FW_VER(IN int enc_id, IN int port_id, OUT char *sr_ver, IN int sr_ver_len)
 * @brief Retrieve SR firmware version.
 *
 * This function retrieves SR firmware version.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Thunderbolt slot id.
 * @param[out] sr_ver pointer to store fw version
 * @param[in] sr_ver_len length of the input sr_ver
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int SR_Get_FW_VER(IN int enc_id, IN int port_id, OUT char *sr_ver, IN int sr_ver_len);

/*@}*/  /* end of SR model Function */

/*@{*/    /* begin of TR model Function */
#define MAX_DISK_IN_CONTROLLER 4
#define MAX_RAID_IN_CONTROLLER 4

typedef enum _TR_TYPE
{
    TR_UNKNOWN  = -1,           /*!< Unknown TR device (-1) */
    TR_004      = 0,            /*!< TR-004 (0) */
    TR_002      = 1,            /*!< TR-002 (1) */
    TR_004U     = 2,            /*!< TR-004U (2) */
    TR_MAX_MODEL_ID,
} TR_TYPE;

typedef enum _TR_RAID_LEVEL
{
    TR_RAID_INVALID = -1,
    TR_RAID0    = 0,            /*!< RAID 0 (0) */
    TR_RAID1    = 1,            /*!< RAID 1 (1) */
    TR_JBOD     = 2,            /*!< JBOD   (2) */
    TR_RAID5    = 5,            /*!< RAID 5 (5) */
    TR_RAID6        = 6,            /*!< RAID 5 (5) */
} TR_RAID_LEVEL;

typedef enum _TR_SWITCH_MODE
{
    TR_SWITCH_PM_MODE   = 0,            /*!< PM mode (Individual Mode)  */
    TR_SWITCH_RAID10    = 1,            /*!< RAID1/10                   */
    TR_SWITCH_RAID0     = 2,            /*!< RAID0                      */
    TR_SWITCH_UNKNOWN_3 = 3,            /*!< Unknown Mode               */
    TR_SWITCH_JBOD      = 4,            /*!< JBOD                       */
    TR_SWITCH_UNKNOWN_5 = 5,            /*!< Unknown Mode               */
    TR_SWITCH_RAID5     = 6,            /*!< RAID5                      */
    TR_SWITCH_SOFTWARE  = 7,            /*!< Software Controll          */
    TR_SWITCH_UNKNOWN   = 0xff,         /*!< Unknown Mode               */
} TR_SWITCH_MODE;

typedef enum _TR_SPARE_ACTION
{
    TR_REMOVE_SPARE = 0,        /*!< Add spare drive    */
    TR_ADD_SPARE    = 1,        /*!< Remove spare drive */
} TR_SPARE_ACTION;

typedef enum _TR_RAID_STATUS
{
    TR_RAID_UNKNOWN     = -1,
    TR_RAID_BROKEN      = 0,
    TR_RAID_DEGRADED    = 1,
    TR_RAID_REBUILD     = 2,
    TR_RAID_NORMAL      = 3,
    TR_RAID_MIGRATION   = 4,
} TR_RAID_STATUS;

typedef enum _TR_RAID_REBUILD_PRIORITY
{
    TR_REBUILD_PRIO_LOWEST      = 1,
    TR_REBUILD_PRIO_LOW         = 2,
    TR_REBUILD_PRIO_MEDIAN      = 3,
    TR_REBUILD_PRIO_HIGH        = 4,
    TR_REBUILD_PRIO_HIGHEST     = 5,
} TR_RAID_REBUILD_PRIORITY;

typedef enum _TR_EVENT
{
    TR_EVENT_DISK_PLUGIN                  = 0,
    TR_EVENT_DISK_PLUGOUT,
    TR_EVENT_RAID_FAILED                  = 10,
    TR_EVENT_RAID_DEGRADED,
    TR_EVENT_RAID_TRY_TO_REBUILD,
    TR_EVENT_RAID_REBUILD_START,
    TR_EVENT_RAID_REBUILD_FAIL,
    TR_EVENT_RAID_REBUILD_DONE,
    TR_EVENT_RAID_CHANGE_REBUILD_PRIORITY,
    TR_EVENT_ENCLOSURE_CONNECTED          = 30,
    TR_EVENT_ENCLOSURE_DISCONNECTED,
    TR_EVENT_ENCLOSURE_CHANGE_SWITCH_MODE,
} TR_EVENT;

/************************************************************************
TL SATA JBOD
************************************************************************/
typedef enum _TL_FAN_SWITCH_MODE
{
    TL_FAN_AUTO 	= 0,           
	TL_FAN_SPEED1	= 1,	//low
	TL_FAN_SPEED2	= 2,
	TL_FAN_SPEED3	= 3,
	TL_FAN_SPEED4	= 4,
	TL_FAN_SPEED5	= 5,	//medium
	TL_FAN_SPEED6	= 6,
	TL_FAN_SPEED7	= 7		//high

} TL_FAN_SWITCH_MODE;

typedef enum _TL_BUZZER_SWITCH_MODE
{
    TL_BUZZER_MUTE 	= 0,           
	TL_BUZZER_NORMAL= 1	
} TL_BUZZER_SWITCH_MODE;

typedef enum _QXP_CLASS
{
    QXP_CLASS_INVALID   = -1, 	// parse error
    QXP_CLASS_400ES     = 0, 	// QXP-400eS
    QXP_CLASS_800ES,     		// QXP-800eS
    QXP_CLASS_1600ES,      	    // QXP-1600eS
    QXP_CLASS_MAX_NUM,
} QXP_CLASS;

typedef enum _TLS_CLASS
{
    TLS_CLASS_INVALID    = -1,  // parse error
    TLS_CLASS_D400S      = 0,   // TL-D400S
    TLS_CLASS_R400S,            // TL-R400S
    TLS_CLASS_D800S,            // TL-D800S
    TLS_CLASS_R1200S,           // TL-R1200S
    TLS_CLASS_D1600S,           // TL-D1600S
    TLS_CLASS_R1600S,           // TL-R1600S
    TLS_CLASS_UNKNOWN_4BAY,     // Unknown 4 bay device
    TLS_CLASS_UNKNOWN_8BAY,     // Unknown 8 bay device
    TLS_CLASS_UNKNOWN_16BAY,    // Unknown 16 bay device
    TLS_CLASS_MAX_NUM,
} TLS_CLASS;

typedef enum _TLUC_CLASS
{
    TLUC_CLASS_INVALID    = -1,  // parse error
    TLUC_CLASS_D800UC      = 0,   // TL-D800UC
    TLUC_CLASS_R1200UC,     	// TL-R1200UC
    TLUC_CLASS_MAX_NUM,
} TLUC_CLASS;

/************************************************************************
 * Define SATA port error status.
 ************************************************************************/
#define SATA_ERROR_GET_SIGNATURE                        0x0001		/* Signature Error                      */
#define SATA_ERROR_INIT_RAID_INFO                       0x0002		/* Init RAID Info Error                 */
#define SATA_ERROR_UPDATE_RAID_INFO_READ                0x0004		/* PIO Read Error                       */
#define SATA_ERROR_UPDATE_RAID_INFO_WRITE               0x0008		/* PIO Write Error                      */
#define SATA_ERROR_UPDATE_RAID_LOG_INFO                 0x0010		/* Read Log Error                       */
#define SATA_ERROR_GET_SMART_INFO                       0x0020		/* Read SMART Info Error                */
#define SATA_ERROR_DISK_DAMAGE                          0x0040		/* Disk Damage Error                    */
#define SATA_ERROR_BAD_SECTOR                           0x0080		/* Bad Sector Error                     */
#define SATA_ERROR_PAGE0_NULL_MEMBER                    0x0100		/* Page 0 is NULL member                */
#define SATA_ERROR_PAGE1_NULL_MEMBER                    0x0200		/* Page 1 is NULL member                */
#define SATA_ERROR_PAGE0_AREA_DAMAGE                    0x0400		/* Page 0 Damage Error                  */
#define SATA_ERROR_PAGE1_AREA_DAMAGE                    0x0800		/* Page 1 Damage Error                  */
#define SATA_ERROR_PAGE0_INFO_BAD_SECTOR                0x1000		/* Page 0 RAID Info Sector Error        */
#define SATA_ERROR_PAGE1_INFO_BAD_SECTOR                0x2000		/* Page 1 RAID Info Sector Error        */

/** 
 * @struct      TR_RAID_CONF
 * @brief       Disk array configuration.
 */
typedef struct _TR_RAID_CONF
{
    int raid_id;                                /*!< Disk array identifier */
    char raid_uuid[TR_MAX_UUID_LEN];            /*!< Disk array UUID */
    TR_RAID_LEVEL raid_level;                   /*!< Disk array level */
    int data_drive_num;                         /*!< The number of data drives */
    PD_DEV_ID data_drives[MAX_DRIVES_SUPPORT];  /*!< The device id array of data drives */
    int spare_drive_num;                        /*!< The number of spare drives */
    PD_DEV_ID spare_drives[MAX_DRIVES_SUPPORT]; /*!< The device id array of spare drives */
    struct _TR_RAID_CONF *next;
} TR_RAID_CONF;

/** 
 * @struct     TR_RAID_INFO
 * @brief      Disk array runtime statistics information.
 */
typedef struct _TR_RAID_INFO 
{
    TR_RAID_LEVEL raid_level;                   /*!< Disk array level */
    int raid_dev_num;                           /*!< The number of data drives */
    int spare_dev_num;                          /*!< The number of spare drives */
    TR_RAID_STATUS status;                      /*!< The status of disk array */
    TR_RAID_REBUILD_PRIORITY prio;              /*!< The rebuild priority of disk array */
    unsigned long long array_size;              /*!< Disk array total capacity, in KB */
    float rebuild_percent;                      /*!< The rebuild percentage of disk array */
} TR_RAID_INFO;

/**
 * @fn int TR_RAID_Create(IN TR_RAID_CONF* TR_raid_confP)
 * @brief Create a h/w raid according to the information in the TR_raid_confP structure.
 *
 * This function creates a h/w raid according to the information in the TR_raid_confP structure.
 * @param[in] TR_raid_confP TR(Turbo Raid) h/w raid descriptor.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Create(IN TR_RAID_CONF* TR_raid_confP);

/**
 * @fn int TR_RAID_Destroy(IN int raid_id)
 * @brief Remove a h/w raid with the specified raid_id.
 *
 * This function removes a h/w raid with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Destroy(IN int raid_id);

/**
 * @fn int TR_RAID_Adjust_Rebuild_Priority(IN int raid_id, IN TR_RAID_REBUILD_PRIORITY prio)
 * @brief Set the rebuild priority of the h/w raid with the specified raid_id.
 *
 * This function sets the rebuild priority of the h/w raid with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[in] prio The rebuild priorty, which is [1..5], but only [2..4] is acceptible by the uppder layer. level-1 is the same as level-2; level-5 is the same as level-4
 * @param[out] controller_id The real controller id the raid id located at
 * @param[out] real_raid_id The real raid id the raid id located at in the JM f/w
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Adjust_Rebuild_Priority(IN int raid_id, IN TR_RAID_REBUILD_PRIORITY prio);

/**
 * @fn int TR_RAID_Get_Info(IN int raid_id, OUT TR_RAID_INFO* TR_raid_infoP)
 * @brief Retrieve the h/w raid status information with the specified raid_id.
 *
 * This function retrieves the h/w raid status information with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[out] TR_raid_infoP The structure used to store the h/w raid status and information.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Get_Info(IN int raid_id, OUT TR_RAID_INFO* TR_raid_infoP);

/**
 * @fn int TR_RAID_Add_Spare(IN int raid_id, IN int port_id)
 * @brief Add a spare drive to the h/w raid with the specified raid_id.
 *
 * This function adds a spare drive to the h/w raid with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[in] port_id The physical disk id in the same TR enclosure the h/w raid located. (1 based)
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Add_Spare(IN int raid_id, IN int port_id);

/**
 * @fn int TR_RAID_Remove_Spare(IN int raid_id, IN int port_id)
 * @brief Remove a spare drive from the h/w raid with the specified raid_id.
 *
 * This function removes a spare drive from the h/w raid with the specified raid_id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[in] port_id The physical disk id in the same TR enclosure the h/w raid located. (1 based)
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Remove_Spare(IN int raid_id, IN int port_id);

/**
 * @fn int TR_RAID_Reset_Spare_Drive_Bitmap(IN int raid_id, IN int add_spare, IN int port_id)
 * @brief Reset the spare drive bitmap of the specified raid id.
 *
 * This function reset the spare drive bitmap of the specified raid id.
 * @param[in] raid_id The h/w raid id, which is assigned when during the creation.
 * @param[in] add_spare The operation. 1 means Add Spare Drive; 0 means Remove Spare Drive
 * @param[in] port_id The physical port id of the spare drive  (1 based)
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int TR_RAID_Reset_Spare_Drive_Bitmap(IN int raid_id, IN int add_spare, IN int port_id);

/**
 * @fn int TR_RAID_Get_Switch_Mode(IN int enc_id, OUT TR_SWITCH_MODE *mode)
 * @brief Get the dipswitch mode of the specified TR enc_sys_id.
 *
 * This function gets the dipswitch mode of the specified TR enc_sys_id.
 * @param[in] enc_id enclosure identifier (0 based).
 * @param[out] mode The dipswitch mode of the specified TR enc_sys_id
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Get_Switch_Mode(IN int enc_id, OUT TR_SWITCH_MODE *mode);

/**
 * @fn int TR_RAID_Get_Port_Status(IN int enc_id, IN int port_id, OUT int *status)
 * @brief Get the dipswitch mode of the specified TR enc_sys_id.
 *
 * This function gets the error status of the specified TR enc_id & port_id.
 * @param[in] enc_id enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[out] status The error status of the specified disk.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Get_Port_Status(IN int enc_id, IN int port_id, OUT int *status);

/**
 * @fn int TR_RAID_Get_Port_Status(IN int enc_id, IN int port_id, OUT int *status)
 * @brief Get the dipswitch mode of the specified TR enc_sys_id.
 *
 * This function gets the error status of the specified TR enc_id & port_id from the cached file.
 * @param[in] enc_id enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[out] status The error status of the specified disk.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Get_Port_Status_By_Cached_File(IN int enc_id, IN int port_id, OUT int *status);

/**
 * @fn int TR_RAID_Handle_Disk_Remove(IN unsigned char *wwn)
 * @brief Handles the disk removal in the specified TR controller.
 *
 * This function handles the disk removal in the specified TR controller.
 * @param[in] wwn Disk WWN.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Handle_Disk_Remove(IN unsigned char *wwn);

/**
 * @fn int TR_RAID_Handle_Disk_Add(IN int controller_id, IN int port_id)
 * @brief Handles the disk addon in the specified TR controller.
 *
 * This function handles the disk removal in the specified TR controller.
 * @param[in] controller_id TR controller ID (0 based).
 * @param[in] port_id Disk channel index (0 based).
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Handle_Disk_Add(IN int controller_id, IN int port_id);

/**
 * @fn int TR_RAID_Handle_Event(IN int controller_id, IN int port_id, IN int enc_id, IN TR_EVENT event, IN int value)
 * @brief Get the dipswitch mode of the specified TR enc_sys_id.
 *
 * This function handles the raid/disk events.
 * @param[in] controller_id TR controller ID (0 based).
 * @param[in] port_id Raid/disk index (0 based).
 * @param[in] enc_id enclosure identifier (0 based).
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int TR_RAID_Handle_Event(IN int controller_id, IN int port_id, IN int enc_id, IN TR_EVENT event, IN int value);

/**
 * @fn int TR_Check_Disk_Relinking(IN char *driveName)
 * @brief Check if the specified drive is linked to a TR enclosure that is under relinking process.
 *
 * This function is used to check if the specified drive is linked to a TR enclosure that is under relinking process.
 * @param[in] drive_name The drive name in the system
 * @retval 0 Disk is not connected to a relinking TR.
 * @retval 1 Disk is connected to a relinking TR.
 */
int TR_Check_Disk_Relinking(IN char *driveName);
/*@}*/  /* end of TR model Function */

/**
 * @defgroup LIB_GPIO General Purpose I/O function
 */

/**
 * @ingroup LIB_GPIO General Purpose I/O function
 */
/*@{*/  /* begin of gpio module */

// The following definition just for user mode application
#define ERROR_NO_ERROR                          0x0000  /*!< Status LED: No error. */
#define ERROR_NO_HD                             0x0001  /*!< Status LED: Disk absent. */
#define ERROR_HD_WRONG_FORMAT                   0x0002  /*!< Status LED: Disk wrong format. */
#define REBUILDING                              0x0020  /*!< Status LED: RAID proceeds data rebuilding. */
#define IMG_UPDATE                              0x0040  /*!< Status LED: Firmware image update. */
#define FORMATTING                              0x0080  /*!< Status LED: Disk formating. */
#define ERROR_HD_BAD_BLOCK                      0x0100  /*!< Status LED: Disk bad block found. */
#define ERROR_NIC_DOWN                          0x0200  /*!< Status LED: NIC down. */
#define DEGRADE                                 0x0400  /*!< Status LED: RAID enters into degrade mode. */
#define ERROR_HD_FULL                           0x1000  /*!< Status LED: Disk full. */
#define ERROR_HD_ALMOST_FULL                    0x2000  /*!< Status LED: Disk almost full. */
#define FAN_ERROR                               0x4000  /*!< Status LED: FAN malfunction. */
#define POWER_ERROR                             0x8000  /*!< Status LED: Power malfunction. */
#define HD_STANDBY                              0x10000 /*!< Status LED: Disk standby. */
#define SYS_SHUTDOWN                            0x20000 /*!< Status LED: System shutdown. */
#define SYS_RESUME                              0x40000 /*!< Status LED: System Resume from standby. */
#ifdef STORAGE_V2
#define ERROR_POOL_HIT_THRESHOLD                ERROR_HD_ALMOST_FULL    /*!< Status LED: Storage pool used capacity hit threshold. */
#define ERROR_POOL_STATUS_ERROR                 ERROR_HD_ALMOST_FULL    /*!< Status LED: Storage pool status error. */
#define ERROR_VOLUME_HIT_THRESHOLD              ERROR_HD_ALMOST_FULL    /*!< Status LED: Volume used capacity hit threshold. */
#define ERROR_VOLUME_STATUS_ERROR               ERROR_HD_ALMOST_FULL    /*!< Status LED: Volume status error. */
#define ERROR_ISCSI_HIT_THRESHOLD               ERROR_HD_ALMOST_FULL    /*!< Status LED: iSCSI lun used capacity hit threshold. */
#define ERROR_VOLUME_FULL                       ERROR_HD_FULL           /*!< Status LED: Flexible volume full. */
#endif

/**
 * @enum        STATUS_LED_COLOR
 * @brief       Status LED color index.
 */
typedef enum _STATUS_LED_COLOR
{
    SLED_RED = 0,                       /*!< Red Status LED index */
    SLED_GREEN,                         /*!< Green Status LED index */
    SLED_RED_GREEN,                     /*!< Red and Green Status LED both */
    SLED_BLINK, 
    SLED_LAST
} STATUS_LED_COLOR;

/**
 * @enum        GPIO_BUTTON
 * @brief       Button index.
 */
typedef enum _GPIO_BUTTON
{
    BUTTON_COPY = 0,            /*!< Copy button index */
    BUTTON_RESET,               /*!< Reset button index */
    BUTTON_LAST
} GPIO_BUTTON;

// The following definition just for GPIO_Enable_Ident_Action().
#define IDENT_ACT_OFF               0x0000  /*!< Ident action: disable all ident actions. */
#define IDENT_ACT_LED_ON            0x0001  /*!< Ident action: locate led on. */
#define IDENT_ACT_BUZZER_ON         0x0002  /*!< Ident action: buzzer on. */
#define IDENT_ACT_LCM_ON            0x0004  /*!< Ident action: lcm light on. */
#define IDENT_ACT_SPEAKER_ON        0x0008  /*!< Ident action: speaker on. */

/**
 * @fn          typedef void (*BUTTON_CALLBACK) (unsigned int context)
 * @brief       Button pressing detection callback function.
 * @param       context Callback parameter which is provided in registration.
 */
typedef void (*BUTTON_CALLBACK) (unsigned int context);

/**
 * @fn int GPIO_Set_NAS_Status_LED(IN int event, IN int status)
 * @brief NAS Status LED function based on specified event and statua.
 *
 * This function will activate the status LED according to the specified event and status.
 * @param[in] event The specified event.
 * @param[in] status The specified status.
 * @retval 0 Success, activate LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Set_NAS_Status_LED(IN int event, IN int status);

/**
 * @fn int GPIO_Enable_Status_LED(IN int enc_id, IN STATUS_LED_COLOR color, IN int enable)
 * @brief Status LED turn on/off function.
 *
 * This function will turn on/off the specified status LED in the front panel.
 * @param[in] enc_id enclosure identifier (0 based).
 * @param[in] color Status LED index in color.
 * @param[in] enable 0/1 To enable/disable the specified color LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Status_LED(IN int enc_id, IN STATUS_LED_COLOR color, IN int enable);

/**
 * @fn int GPIO_Enable_Status_LED_BLINK(IN int enc_id, IN STATUS_LED_COLOR color, IN int enable)
 * @brief Status LED blink turn on/off function.
 *
 * This function will turn on/off the specified status LED blink in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] color Status LED index in color.
 * @param[in] enable 0/1 To enable/disable the specified color LED blink.
 * @retval 0 Success, turn on/off LED blink completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Status_LED_BLINK(IN int enc_id, IN STATUS_LED_COLOR color, IN int enable);

/**
 * @fn int GPIO_Enable_Power_LED(IN int enc_id, IN int enable)
 * @brief USB (Blue) LED turn on/off function.
 *
 * This function will turn on/off the Power (Green) LED in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the USB LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Power_LED(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_Power_LED_BLINK(IN int enc_id, IN int enable)
 * @brief USB (Blue) LED blink turn on/off function.
 *
 * This function will turn on/off the Power (Green) LED blink in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the LED blink.
 * @retval 0 Success, turn on/off LED blink completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Power_LED_BLINK(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_Standby_LED(IN int enc_id, IN int enable)
 * @brief Standby LED turn on/off function.
 *
 * This function will turn on/off the standby led when enter s3 mode.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the standby led.
 * @retval 0 Success, turn on/off standy led completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Standby_LED(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_Breath_LED(IN int enc_id, IN int enable)
 * @brief Breath (Blue) LED turn on/off function.
 *
 * This function will turn on/off the Breath (Blue) LED in the front panel.
 * @param[in] enc_id enclosure identifier.
 * @param[in] enable 0/1 To enable/disable the Breath LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Breath_LED(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_USB_LED(IN int enc_id, IN int enable)
 * @brief USB (Blue) LED turn on/off function.
 *
 * This function will turn on/off the USB (Blue) LED in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the USB LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_USB_LED(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_USB_LED_BLINK(IN int enc_id, IN int enable)
 * @brief USB (Blue) LED blink turn on/off function.
 *
 * This function will turn on/off the USB (Blue) LED blink in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the LED blink.
 * @retval 0 Success, turn on/off LED blink completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_USB_LED_BLINK(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_BREATHING_LED(IN int enc_id, IN int enable)
 * @brief USB (Blue) LED turn on/off function.
 *
 * This function will turn on/off the BREATHING LED.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the BREATHING LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_BREATHING_LED(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_BREATHING_LED_BLINK(IN int enc_id, IN int enable)
 * @brief BREATHING LED blink turn on/off function.
 *
 * This function will turn on/off the BREATHING LED blink.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the LED blink.
 * @retval 0 Success, turn on/off LED blink completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_BREATHING_LED_BLINK(IN int enc_id, IN int enable);

/**
 * @fn int GPIO_Enable_Ident_Action(IN int enc_id, IN int action)
 * @brief Enclosure Ident LED turn on/off function.
 *
 * This function will turn on/off the specified Enclosure Identify LED in the front panel, only for JBOD
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable 0/1 To enable/disable the specified color LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_Ident_Action(IN int enc_id, IN int action);

/**
 * @fn int GPIO_Get_Ident_Action(IN int enc_id)
 * @brief Enclosure Ident LED status retrieval function.
 *
 * This function will retrieve the specified Enclosure Identify LED status in the front panel, only for JBOD
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 TRUE, the Identify LED is turned on.
 * @retval 0 FALSE, the Identify LED is turned off.
 */
int GPIO_Get_Ident_Action(IN int enc_id);

/**
 * @fn int GPIO_Enable_HDD_ERR_LED(IN int enc_id, IN int port_id, IN int enable)
 * @brief HDD Error LED turn on/off function.
 *
 * This function will turn on/off the specified HDD Error LED in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[in] enable 0/1 To enable/disable the specified color LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_HDD_ERR_LED(IN int enc_id, IN int port_id, IN int enable);

/**
 * @fn int GPIO_Enable_HDD_Ident_LED(IN int enc_id, IN int port_id, IN int enable, IN int pd_error)
 * @brief HDD Ident LED turn on/off function.
 *
 * This function will turn on/off the specified HDD Ident LED in the front panel,only for JBOD
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[in] enable 0/1 To enable/disable the specified color LED.
 * @param[in] pd_error 0/1 To set error led when clear Ident LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_HDD_Ident_LED(IN int enc_id, IN int port_id, IN int enable, IN int pd_error);

/**
 * @fn int GPIO_Enable_HDD_Present_LED(IN int enc_id, IN int port_id, IN int enable)
 * @brief HDD Present LED turn on/off function.
 *
 * This function will turn on/off the specified HDD Present LED in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[in] enable 0/1 To enable/disable the specified present LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_HDD_Present_LED(IN int enc_id, IN int port_id, IN int enable);

/**
 * @fn int GPIO_Enable_HDD_Active_LED(IN int enc_id, IN int port_id, IN int enable)
 * @brief HDD Active LED turn on/off function.
 *
 * This function will turn on/off the specified HDD Active LED in the front panel.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Disk channel index (1 based).
 * @param[in] enable 0/1 To enable/disable the specified active LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_HDD_Active_LED(IN int enc_id, IN int port_id, IN int enable);

/**
 * @fn int GPIO_Enable_10G_Present_LED(IN int enable)
 * @brief 10G NIC present LED turn on/off function.
 *
 * This function will turn on/off the specified 10G NIC present LED in the front panel.
 * @param[in] enable 0/1 To enable/disable the 10G NIC present LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_10G_Present_LED(IN int enable);

/**
 * @fn int GPIO_Enable_EXP_Present_LED(IN int enable)
 * @brief EXP present LED turn on/off function.
 *
 * This function will turn on/off the specified EXP present LED in the front panel.
 * @param[in] enable 0/1 To enable/disable the EXP present LED.
 * @retval 0 Success, turn on/off LED completely.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_EXP_Present_LED(IN int enable);

/**
 * @fn int GPIO_Does_Eject_Button_Press(IN int enc_id)
 * @brief Eject button pressing detection function.
 *
 * This function will detect whether eject button is pressed.
 * @param[in] enc_id enclosure identifier.
 * @retval 1 Eject button is pressed.
 * @retval 0 Eject button is not pressed.
 * @retval -1 General error.
 */
int GPIO_Does_Eject_Button_Press(IN int enc_id);

/**
 * @fn int GPIO_Does_Copy_Button_Press(IN int enc_id)
 * @brief Copy button pressing detection function.
 *
 * This function will detect whether Copy button is pressed.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 Copy button is pressed.
 * @retval 0 Copy button is not pressed.
 */
int GPIO_Does_Copy_Button_Press(IN int enc_id);

/**
 * @fn int GPIO_Does_Reset_Button_Press(IN int enc_id)
 * @brief Reset button pressing detection function.
 *
 * This function will detect whether Reset button is pressed.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 Reset button is pressed.
 * @retval 0 Reset button is not pressed.
 */
int GPIO_Does_Reset_Button_Press(IN int enc_id);

/**
 * @fn int GPIO_Get_Power_Status(IN int enc_id, IN int pwr_id, OUT PWR_STATUS* statusP)
 * @brief Retrieve the specified power status.
 *
 * This function will query the specified power status from the system dependent layer.
 * @param[in] enc_id enclosure identifier (0 based).
 * @param[in] pwr_id power supply index (1 based).
 * @param[out] statusP power status.
 * @retval 0 Success, query the power status successfully.
 * @retval -1 Generic failure.
 */
int GPIO_Get_Power_Status(IN int enc_id, IN int pwr_id, OUT PWR_STATUS* statusP);

/**
 * @fn int GPIO_Does_Case_Cover_Open(IN int enc_id)
 * @brief Case cover open detection function.
 *
 * This function will detect whether case cover is opened
 * @param[in] enc_id Enclosure identifier (0 based).
 * @retval 1 Case cover is opened.
 * @retval 0 Case cover is closed.
 * @retval -1 Fail
 */
int GPIO_Does_Case_Cover_Open(IN int enc_id);

/**
 * @fn int GPIO_Enable_LED_In_Night_Mode(IN int enc_id, IN int mode)
 * @brief Enable LED in night mode.
 *
 * This function will enable /disable all LED in night mode.
 * @param[in] enc_id enclosure identifier.
 * @param[in] mode support off/half/full different led brightness.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int GPIO_Enable_LED_In_Night_Mode(IN int enc_id, IN int mode);


/*@}*/  /* end of gpio module */

/**
 * @defgroup LIB_BBU Backup battery Unit function
 */

/**
 * @ingroup LIB_BBU Backup battery Unit functioneneral
 */
/*@{*/  /* begin of bbu module */

/**
 * @fn int HAL_MB_Set_BBU_Status_LED(IN STATUS_LED_COLOR color, IN int blink, IN int enable)
 * @brief Enable/Blink BBU status led.
 *
 * This function will enable/blink green/red led for BBU.
 * @param[in] color Specified which led should be actived.
 * @param[in] blink blink or on.
 * @param[in] enable enable/disable.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_BBU_Status_LED(IN STATUS_LED_COLOR color, IN int blink, IN int enable);

/**
 * @fn int HAL_MB_Charge_BBU(IN int enable)
 * @brief Enable/Disable charge BBU.
 *
 * This function will enable/disable charge BBU.
 * @param[in] enable enable/disable.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Charge_BBU(IN int enable);

/**
 * @fn int HAL_MB_Discharge_BBU(IN int enable)
 * @brief Enable/Disable discharge BBU.
 *
 * This function will enable/disable discharge BBU.
 * @param[in] enable enable/disable.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Discharge_BBU(IN int enable);

/**
 * @struct      BBU_STATUS_INFO
 * @brief       BBU status info from gas gauge i2c device
 */
typedef struct _BBU_STATUS_INFO
{
    float temperature_C;
    short voltage_mV;
    short current_mA;
    short capacity_mAh;
    short full_capacity_mAh;
    short manufacture_date;
    short serial_number;
    unsigned char manufacturer_name[32];
    unsigned char device_name[32];
} BBU_STATUS_INFO;

/**
 * @enum        BBU_LEARNING_STAGE
 * @brief       BBU learning stage.
 */
typedef enum _BBU_LEARNING_STAGE
{
    DEFAULT_LEARN_STAGE = 0,       /*!< DEFAULT LEARNING STAGE. */
    CHARGE_TO_FULL,                /*!< LEARNING MODE CHARGING. */
    DISCHARGE_TO_LOW,              /*!< LEARNING MODE DISCHARGE. */
    RECHARGE_TO_FULL,              /*!< LEARNING MODE RECHARGING. */
} BBU_LEARNING_STAGE;

/**
 * @struct      BBU_LEARNING_INFO
 * @brief       BBU learning progress and stage
 */
typedef struct _BBU_LEARNING_INFO
{
    float learning_progress;              /*!< BBU learning mode progress. */
    BBU_LEARNING_STAGE learning_stage;    /*!< BBU learning mode stage. */
} BBU_LEARNING_INFO;

/**
 * @fn int HAL_MB_Get_BBU_Status(OUT BBU_STATUS_INFO* status)
 * @brief Get BBU status.
 *
 * This function will get BBU status.
 * @param[in] status get BBU status structure.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_BBU_Status(OUT BBU_STATUS_INFO* status);

/**
 * @fn int HAL_MB_Test_BBU(IN int enable)
 * @brief Enable/Disable test BBU.
 *
 * This function will enable/disable test BBU.
 * @param[in] enable enable/disable test BBU
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Test_BBU(IN int enable);

/**
 * @fn int HAL_MB_Is_BBU_Exist(void)
 * @brief Check BBU exist or not.
 *
 * This function will check BBU exist or not.
 * @retval 1 BBU exist.
 * @retval 0 BBU not exist.
 */
int HAL_MB_Is_BBU_Exist(void);

/**
 * @fn int BBU_Learning_Enable(IN int enable)
 * @brief Trigger hal_daemon Enable/Disable BBU learning mode.
 *
 * This function will send event to hal_daemon and
 * enable/disable BBU learning mode.
 * @retval 0 Success, send event successfully.
 * @retval !0 Generic failure, send event fail.
 */
int BBU_Learning_Enable(IN int enable);

/**
 * @fn int BBU_Learning_Progress(OUT BBU_LEARNING_INFO* learning_infoP)
 * @brief Check BBU learning mode progress.
 *
 * This function will check BBU learning mode progress.
 * @retval 0 check BBU learning progress success.
 * @retval -1 check BBU learning progress fail.
 */
int BBU_Learning_Progress(OUT BBU_LEARNING_INFO* learning_infoP);

/*@}*/  /* end of bbu module */


/**
 * @defgroup LIB_HM Hardware Monitor function
 */
/*@{*/  /* begin of hardware monitor module */

/**
 * @enum        PR_MODE
 * @brief       Power recovery mode.
 */
typedef enum _PR_MODE
{
    PR_LAST_STATE = 0,          /*!< Back to the last state while power lost. */
    PR_ALWAYS_ON,               /*!< Always power off after power resume. */
    PR_ALWAYS_OFF,              /*!< Always power on after power resume. */
    PR_ALWAYS_OFF_NO_UPDATE_CMOS,/*!< For Super IO not support Last State. */
    PR_ALWAYS_ON_NO_UPDATE_CMOS,/*!< For Super IO not support Last State. */
    PR_LAST_STATE_WA,           /*!< Back to the last state while power lost for WA. */
    PR_ALWAYS_OFF_WA,           /*!< Always power on after power resume for WA. */
} PR_MODE;

/**
 * @enum        TEMP_INDEX
 * @brief       Thermal sensor location index.
 */
typedef enum _TEMP_INDEX
{
    TEMP_CPU1 = 0,              /*!< CPU1 thermal sensor index */
    TEMP_CPU2,                  /*!< CPU2 thermal sensor index */
    TEMP_SYSTEM1 = 5,           /*!< System1 thermal sensor index*/
    TEMP_SYSTEM2,           	/*!< System2 thermal sensor index */
    TEMP_SYSTEM3,           	/*!< System3 thermal sensor index */
    TEMP_SYSTEM4,           	/*!< System4 thermal sensor index */
    TEMP_POWER1 = 10,           /*!< Power1 temperature index */
    TEMP_POWER2,                /*!< Power2 temperature index */
    TEMP_ENV_SENSOR1 = 15,       /*! < Back Plane themal sensor index 1 */
    TEMP_ENV_SENSOR24 = 38,      /*! < Back Plane themal sensor index 24 */
    TEMP_LAST,                  /*!< Max temp index */
} TEMP_INDEX;

/**
 * @enum        FAN_INDEX
 * @brief       FAN location index.
 */
typedef enum _FAN_INDEX
{
    FAN_ALL = -1,
    FAN_SYSTEM1 = 0,    /*!< System FAN1 index */
    FAN_SYSTEM2,        /*!< System FAN2 index */
    FAN_SYSTEM3,        /*!< System FAN3 index */
    FAN_SYSTEM4,        /*!< System FAN4 index */
    FAN_SYSTEM5,        /*!< System FAN5 index */
    FAN_CPU1 = 6,         /*!< CPU FAN1 index */
    FAN_CPU2,              /*!< CPU FAN2 index */
    FAN_POWER1 = 10,    /*!< Power1 FAN index */
    FAN_POWER2,         /*!< Power2 FAN index */
    FAN_REGION1_1 = 20,     /*!< System chassis Region1 FAN1 index */
    FAN_REGION1_2 = 21,     /*!< System chassis Region1 FAN2 index */
    FAN_REGION1_3 = 22,     /*!< System chassis Region1 FAN3 index */
    FAN_REGION1_4 = 23,     /*!< System chassis Region1 FAN4 index */
    FAN_REGION1_5 = 24,     /*!< System chassis Region1 FAN5 index */
    FAN_REGION1_6 = 25,     /*!< System chassis Region1 FAN6 index */
    FAN_REGION2_1 = 30,     /*!< System chassis Region2 FAN1 index */
    FAN_REGION2_2 = 31,     /*!< System chassis Region2 FAN2 index */
    FAN_REGION2_3 = 32,     /*!< System chassis Region2 FAN3 index */
    FAN_REGION2_4 = 33,     /*!< System chassis Region2 FAN4 index */
    FAN_REGION2_5 = 34,     /*!< System chassis Region2 FAN5 index */
    FAN_REGION2_6 = 35,     /*!< System chassis Region2 FAN6 index */
    FAN_LAST
} FAN_INDEX;

/**
 * @enum        VOL_INDEX
 * @brief       Voltage sensor index.
 */
typedef enum _VOL_INDEX
{
    VOL_VCORE = 0,              /*!< VCORE sensor index */
    VOL_1_8V,                   /*!< 1.8V sensor index */
    VOL_3_3V,                   /*!< 3.3V sensor index */
    VOL_5V,             /*!< 5V sensor index */
    VOL_12V,                    /*!< 12V sensor index */
    VOL_1_5V,                   /*!< 1.5V sensor index */
    VOL_1_05V,                  /*!< 1.05V sensor index */
    VOL_FSB_VIT,                /*!< FSB sensor index */
    VOL_VBAT,                   /*!< VBAT sensor index */
    VOL_LAST
} VOL_INDEX;

/**
 * @enum        FAN_SPEED
 * @brief       FAN speed constant.
 */
#if 0
typedef enum _FAN_SPEED
{
    FS_STOP = 0,        /*!< Stop fan */
    FS_SILENT,          /*!< Silent */
    FS_LOW,             /*!< Low speed */
    FS_MEDIUM,          /*!< Medium speed */
    FS_HIGH,            /*!< High speed */
    FS_FULLY_SPEED,     /*!< Fully speed */
    FS_LAST
} FAN_SPEED;
#endif
typedef enum _FAN_SPEED
{
    FS_INVALID = -1,    /*!< Invalid fan speed */
    FS_STOP = 0,        /*!< Stop fan */
    FS_LOWEST,          /*!< the lowest speed */
    FS_SECOND_LOWEST,   /*!< the second lowest speed */
    FS_THIRD_LOWEST,    /*!< the third lowest speed */
    FS_MEDIUM,          /*!< the intermediate speed */
    FS_THIRD_HIGHEST,   /*!< the third highest speed */
    FS_SECOND_HIGHEST,  /*!< the second highest speed */
    FS_HIGHEST,         /*!< the highest speed */
    FS_AUTO_MODE,       /*!< the auto mode */
    FS_LAST,
} FAN_SPEED;

/**
 * @fn int HM_Get_Temperature(IN int enc_id, IN TEMP_INDEX temp_index, OUT double *dValueP)
 * @brief System temperature retrieval function.
 *
 * This function will return the specified system temperature in centigrade.
 * This API is used by hal_daemon only to update the real Temperature to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_idx Specified the fan region
 * @param[out] dValueP The pointer of double variable to store the return system temperature.
 * @retval 0 Success, Specified temperature returned.
 * @retval -1 Generic failure.
 */
int HM_Get_Temperature(IN int enc_id, IN TEMP_INDEX temp_index, OUT double *dValueP);

/**
 * @fn int HM_Get_Region_Temperature(IN int enc_id, IN int region_index, OUT double *dValueP)
 * @brief System temperature retrieval function.
 *
 * This function will return the specified system region temperature in centigrade.
 * This API is used by hal_daemon only to update the real Temperature to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] region_index Specified the temp region
 * @param[out] dValueP The pointer of double variable to store the return system region temperature.
 * @retval 0 Success, Specified temperature returned.
 * @retval -1 Generic failure.
 */
int HM_Get_Region_Temperature(IN int enc_id, IN int region_index, OUT double *dValueP);

/**
 * @fn int HM_Get_NIC_Temperature(IN int enc_id, IN int port_id, OUT double *dValueP)
 * @brief System temperature retrieval function.
 *
 * This function will return temperature of the specified NIC interface in centigrade.
 * This API is used to get the real Temperature of the specified nic interface.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Specified NIC ID
 * @param[out] dValueP The pointer of double variable to store the temperature.
 * @retval 0 Success, Specified temperature returned.
 * @retval -1 Generic failure.
 */
int HM_Get_NIC_Temperature(IN int enc_id, IN int port_id, OUT double *dValueP);

/**
 * @fn int HM_Temperature_Calibrate(IN int enc_id, IN FAN_REGION region_idx, IN double dValue, IN int is_ssd)
 * @brief System temperature calibration function.
 *
 * This function will set the specified system temperature in centigrade.
 * This API is used by hal_daemon only to calibrate the real Temperature to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] temp_index The thermal identifier.
 * @param[in] dValue The specified temperature value.
 * @param[in] region_idx FAN control region index.
 * @param[in] is_ssd Is SSD or not.
 * @retval 0 Success, Specified temperature is set.
 * @retval -1 Generic failure.
 */
int HM_Temperature_Calibrate(IN int enc_id, IN FAN_REGION region_idx, IN double dValue, IN int is_ssd);

/**
 * @fn int HM_CPU_Temperature_Feedback(IN int enc_id, IN TEMP_INDEX temp_index, IN double dValue)
 * @brief System temperature calibration function.
 *
 * This function will set the specified CPU temperature in centigrade to peripherals that need
 * this data.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] temp_index The thermal identifier.
 * @param[in] dValue The specified temperature value.
 * @retval 0 Success, Specified temperature is set.
 * @retval -1 Generic failure.
 */
int HM_CPU_Temperature_Feedback(IN int enc_id, IN TEMP_INDEX temp_index, IN double dValue);

/**
 * @fn int HM_Get_Voltage(IN int enc_id, IN VOL_INDEX vol_index, OUT double *dValueP)
 * @brief System voltage retrieval function.
 *
 * This function will return the specified system voltage.
 * This API is used by hal_daemon only to update the real Voltage to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] vol_index The voltage identifier.
 * @param[out] dValueP The pointer of double variable to store the return system voltage.
 * @retval 0 Success, Specified voltage returned.
 * @retval -1 Generic failure.
 */
int HM_Get_Voltage(IN int enc_id, IN VOL_INDEX vol_index, OUT double *dValueP);

/**
 * @fn int HM_Get_Fan(IN int enc_id, IN FAN_INDEX fan_index, OUT unsigned int *dwValueP, OUT unsigned int *fan_fail)
 * @brief Fan tachometer retrieval function.
 *
 * This function will return the specified fan tachometer value.
 * This API is used by hal_daemon only to update the real Fan status to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] fan_index The fan identifier.
 * @param[out] dwValueP The pointer of double word variable to store the return tachometer value in RPM.
 * @param[out] fan_fail current fan status, 1--> fail
 * @retval 0 Success, Specified fan tachometer returned.
 * @retval -1 Generic failure.
 */
int HM_Get_Fan(IN int enc_id, IN FAN_INDEX fan_index, OUT unsigned int *dwValueP, OUT unsigned int *fan_fail);

/**
 * @fn int HM_Set_Fan_Speed(IN int enc_id, IN FAN_INDEX fan_index, IN FAN_SPEED fan_speed)
 * @brief Fan speed control function.
 *
 * This function will control the fan in the specified speed value.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] fan_index The fan identifier.
 * @param[in]  fan_speed The specified fan speed.
 * @retval 0 Success, Specified fan speed is configured OK.
 * @retval -1 Generic failure.
 */
int HM_Set_Fan_Speed(IN int enc_id, IN FAN_INDEX fan_index, IN FAN_SPEED fan_speed);

/**
 * @fn int HM_Set_Fan_Speed_By_PWM(IN int enc_id, IN FAN_INDEX fan_index, IN int pwm)
 * @brief Fan speed control function.
 *
 * This function will control the fan in the specified pwm value.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] fan_index The fan identifier.
 * @param[in] pwm The specified pwm value.
 * @retval 0 Success, Specified fan speed is configured OK.
 * @retval -1 Generic failure.
 */
int HM_Set_Fan_Speed_By_PWM(IN int enc_id, IN FAN_INDEX fan_index, IN int pwm);

/**
 * @fn int HM_Set_FanError_Led(IN int enc_id, IN int mode)
 * @brief Thunderbolt JBOD fan error Led control function.
 *
 * This function will activate the Fan Error LED according to the specified mode.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] mode The display mode of the led.
 * @retval 0 Success.
 * @retval -1 Fail to set the led.
 */
int HM_Set_FanError_Led(IN int enc_id, IN int mode);

/**
 * @fn int HM_Set_Overheat_Led(IN int enc_id, IN int mode)
 * @brief Thunderbolt JBOD Overheat Led control function.
 *
 * This function will activate the Overheat LED according to the specified mode.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] mode The display mode of the led.
 * @retval 0 Success.
 * @retval -1 Fail to set the led.
 */
int HM_Set_Overheat_Led(IN int enc_id, IN int mode);

/**
 * @fn int HM_Set_EUP_State(IN int enc_id, IN int enable)
 * @brief EUP state modification function.
 *
 * This function will change the energy-using-product state.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable EUP setting.
 * @retval 0 Success, change EUP state OK.
 * @retval -1 Generic failure.
 */
int HM_Set_EUP_State(IN int enc_id, IN int enable);

/**
 * @fn int HM_Get_EUP_State(IN int enc_id, OUT int* enableP)
 * @brief EUP state retrieve function.
 *
 * This function will retrieve the energy-using-product state.
 * This API is used by hal_daemon only to update the real EuP status to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] enableP EUP enable status.
 * @retval 0 Success, retrieve EUP state OK.
 * @retval -1 Generic failure.
 */
int HM_Get_EUP_State(IN int enc_id, OUT int* enableP);

/**
 * @fn int HM_Set_Power_Recovery_Mode(IN int enc_id, IN PR_MODE mode)
 * @brief Set the power recovery mode.
 *
 * This function will change the specified power recovery mode of an enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] mode power recovery mode.
 * @retval 0 Success, change the power recovery mode completely.
 * @retval -1 Generic failure.
 */
int HM_Set_Power_Recovery_Mode(IN int enc_id, IN PR_MODE mode);

/**
 * @fn int HM_Get_Power_Recovery_Mode(IN int enc_id, OUT PR_MODE* modeP)
 * @brief Retrieve the power recovery mode.
 *
 * This function will retrieve the specified power recovery mode of an enclosure.
 * This API is used by hal_daemon only to update the real Power recovery mode to internal spool.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] modeP A PR_MODE pointer to accomodate the retrieved power recovery mode.
 * @retval 0 Success, retrieve the power recovery mode completely.
 * @retval -1 Generic failure.
 */
int HM_Get_Power_Recovery_Mode(IN int enc_id, OUT PR_MODE* modeP);

/**
 * @fn HM_Set_Power_Button_Mode(IN int enc_id, IN int gpio_mode)
 * @brief Set Power Button mode.
 *
 * This function will set power button mode to gpio/PSIN#
 * @param[in] enc_id
 * @param[in] gpio_mode mode selection.
 * @retval 0 Success.
 * @retval < 0 Generic failure.
 */
int HM_Set_Power_Button_Mode(IN int enc_id, IN int gpio_mode);

/**
 * @fn int HM_Get_Power_Button_Status(IN int enc_id, OUT int *press)
 * @brief Get Power Button status.
 *
 * This function will get power button status.
 * This API is used by hal_daemon only to update the real Power Button status to internal spool.
 * @param[in] enc_id
 * @param[out] press check power button is pressed or not.
 * @retval 0 Success.
 * @retval < 0 Generic failure.
 */
int HM_Get_Power_Button_Status(IN int enc_id, OUT int *press);

/**
 * @fn int HM_Set_WOL(IN int enc_id, IN int enable)
 * @brief Send wol command to MCU or PIC if support
 *
 * This function will set wol command to MCU or PIC of an enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] enable WOL enable/disable.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_WOL(IN int enc_id, IN int enable);

/**
 * @fn int HM_Get_WOL(IN int enc_id, OUT int *press)
 * @brief Send wol command to MCU or PIC if support
 *
 * This function will get wol status command to MCU or PIC of an enclosure.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] press check WOL flag is pressed or not.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Get_WOL(IN int enc_id, OUT int *press);

/**
 * @fn int HM_Set_C2F_Param(IN int enc_id, IN int enable, IN C2F_RSVD_SIZE size)
 * @brief C2F parameters modification function
 *
 * This function will set related C2F parameters to CMOS.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] enable C2F function enable or disable
 * @param[in] size reserved memory size
 * @retval 0 Success, change C2F parameter OK.
 * @retval -1 Generic failure.
 */
int HM_Set_C2F_Param(IN int enc_id, IN int enable, IN C2F_RSVD_SIZE size);

/**
 * @fn int HM_Get_C2F_Param(IN int enc_id, OUT int* enableP, OUT C2F_RSVD_SIZE* sizeP)
 * @brief C2F parameters retrieve function.
 *
 * This function will retrieve C2F parameter from CMOS.
 * @param[in] enc_id Enclosure identifier.
 * @param[out] enableP C2F function status.
 * @param[out] sizeP C2F reserved memory size.
 * @retval 0 Success, retrieve C2F parameter OK.
 * @retval -1 Generic failure.
 */
int HM_Get_C2F_Param(IN int enc_id, OUT int* enableP, OUT C2F_RSVD_SIZE* sizeP);

/**
 * @fn int HM_Set_LED_Brightness_By_PWM(IN int enc_id, IN int value)
 * @brief LED brightness control function.
 *
 * This function will control led brightness in the specified pwm value.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] value The specified value[0-100]
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_LED_Brightness_By_PWM(IN int enc_id, IN int value);

/**
 * @fn int HM_Init_Audio_Board(IN int enc_id, IN int init)
 * @brief Init Audio Board
 *
 * This function will init Audio board if support
 * @param[in] enc_id Enclosure identifier.
 * @param[in] init do initialization.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_Audio_Board(IN int enc_id, IN int init);

/**
 * @fn int HM_Init_Temp(IN int enc_id, IN HEAT_SOURCE heat, IN int hyst_temp_low, IN int hyst_temp_high, IN int critical_temp, IN int super_crit_temp1, IN int super_cirt_temp2)
 * @brief Init EC Tfan temperature parameters.
 *
 * This function will init EC Tfan temperature parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] heat The index of heat source.
 * @param[in] hyst_temp_low Hysteresis temperature low.
 * @param[in] hyst_temp_high Hysteresis temperature high.
 * @param[in] critical_temp Critical temperature.
 * @param[in] super_crit_temp1 Super critical temperature1.
 * @param[in] super_crit_temp2 Super critical temperature2.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_Temp(IN int enc_id, IN HEAT_SOURCE heat, IN int hyst_temp_low, IN int hyst_temp_high, IN int critical_temp, IN int super_crit_temp1, IN int super_cirt_temp2);

/**
 * @fn int HM_Init_Temp_Offset(IN int enc_id, IN int cpu_formula, IN int cpu_offset, IN int sys_offset)
 * @brief Init EC cpu/system temperature offset related parameters.
 *
 * This function will init EC cpu/system temperature offset parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] cpu_formula The specified cpu formual, set -1 if not use formula
 * @param[in] cpu_offset The specified cpu offset temperature[-128 ~ 127]
 * @param[in] sys_offset The specified system offset temperature[-128 ~ 127]
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_Temp_Offset(IN int enc_id, IN int cpu_formula, IN int cpu_offset, IN int sys_offset);

/**
 * @fn int HM_Init_EC_QFan_PWM(IN int enc_id, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm)
 * @brief Init EC QFAN pwm related parameters.
 *
 * This function will init EC QFAN pwm related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] stop_pwm The specified qfan stop pwm value[0-255]
 * @param[in] low_pwm The specified qfan low pwm value[0-255]
 * @param[in] mid_pwm The specified qfan mid pwm value[0-255]
 * @param[in] high_pwm The specified qfan high pwm value[0-255]
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_QFan_PWM(IN int enc_id, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm);

/**
 * @fn int HM_Init_EC_TFan_PWM(IN int enc_id, IN FAN_REGION region, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm)
 * @brief Init EC TFAN pwm related parameters.
 *
 * This function will init EC TFAN pwm related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @paran[in] region The index of fan region.
 * @param[in] stop_pwm The specified tfan stop pwm value[0-255]
 * @param[in] low_pwm The specified tfan low pwm value[0-255]
 * @param[in] mid_pwm The specified tfan mid pwm value[0-255]
 * @param[in] high_pwm The specified tfan high pwm value[0-255]
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_TFan_PWM(IN int enc_id, IN FAN_REGION region, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm);

/**
 * @fn int HM_Init_EC_TFan_Parameter(IN int enc_id, IN FAN_REGION region, IN int normal_duty)
 * @brief Init EC TFAN parameters.
 *
 * This function will init EC TFAN parameters.
 * @param[in] enc_id Enclosure identifier.
 * @paran[in] region The index of fan region.
 * @param[in] normal_duty FAN normal duty.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_TFan_Parameter(IN int enc_id, IN FAN_REGION region, IN int normal_duty);

/**
 * @fn int HM_Init_EC_TFAN_PWM(IN int enc_id, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm)
 * @brief Init EC TFAN pwm related parameters.
 *
 * This function will init EC TFAN pwm related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] stop_pwm The specified qfan stop pwm value[0-255]
 * @param[in] low_pwm The specified qfan low pwm value[0-255]
 * @param[in] mid_pwm The specified qfan mid pwm value[0-255]
 * @param[in] high_pwm The specified qfan high pwm value[0-255]
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_TFAN_PWM(IN int enc_id, IN int stop_pwm, IN int low_pwm, IN int mid_pwm, IN int high_pwm);


/**
 * @fn int HM_Init_EC_QFan_Auto_Temp(IN int enc_id, IN int low_sys_temp, IN int high_sys_temp,
                                       IN int high_cpu_temp, IN int high_hdd_temp)
 * @brief Init EC QFAN auto mode temperature related parameters.
 *
 * This function will init EC QFAN auto mode temperature related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] low_sys_temp The specified qfan low system temperature value
 * @param[in] high_sys_temp The specified qfan high system temperature value
 * @param[in] high_cpu_temp The specified qfan high cpu temperature value
 * @param[in] high_hdd_temp The specified qfan high hdd temperature value
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_QFan_Auto_Temp(IN int enc_id, IN int low_sys_temp, IN int high_sys_temp,
                                       IN int high_cpu_temp, IN int high_hdd_temp);

/**
 * @fn int HM_Set_EC_QFan_Auto(IN int enc_id, IN int formula)
 * @brief Active EC QFAN auto mode
 *
 * This function will active EC QFAN auto mode
 * @param[in] enc_id Enclosure identifier.
 * @param[in] formula The specified EC formula.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_QFan_Auto(IN int enc_id, IN int formula);

/**
 * @fn int HM_Set_EC_QFan_Custom(IN int enc_id,IN int stop_sys_temp, IN int low_sys_temp, IN int high_sys_temp, IN FAN_REGION region)
 * @brief Active EC QFAN auto mode but use custom temperature parameter.
 *
 * This function will active EC QFAN auto mode but use custom temperature parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] stop_sys_temp The specified system stop temperature value, -1 if no change.
 * @param[in] low_sys_temp The specified system low temperature value, -1 if no change.
 * @param[in] high_sys_temp The specified system high temperature value, -1 if no change.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_QFan_Custom(IN int enc_id,IN int stop_sys_temp, IN int low_sys_temp, IN int high_sys_temp, IN FAN_REGION region);

/**
 * @fn int HM_Set_EC_QFan_Manual(IN int enc_id,IN FAN_SPEED speed)
 * @brief Active EC QFAN MANUAL mode
 *
 * This function will active EC QFAN manual mode
 * @param[in] enc_id Enclosure identifier.
 * @param[in] speed The specified EC QFAN speed.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_QFan_Manual(IN int enc_id,IN FAN_SPEED speed);

/**
 * @fn int HM_Init_EC_7Level_PWM(IN int enc_id)
 * @brief Init EC 7-level Fan PWM related parameters.
 *
 * This function will init EC 7-level Fan PWM related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_7Level_PWM(IN int enc_id);

/**
 * @fn int HM_Init_EC_7Level_Temp(IN int enc_id, IN int cpu_hysteresis, IN int cpu_warning, IN int cpu_critical,
                                        IN int sys_hysteresis, IN int sys_warning, IN int sys_critical,
                                        IN int hdd_hysteresis, IN int hdd_warning, IN int hdd_critical)
 * @brief Init EC 7-level auto mode temperature related parameters.
 *
 * This function will init EC 7-level auto mode temperature related parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] cpu_hysteresis The specified cpu hysteresis temperature value
 * @param[in] cpu_warning The specified cpu warning temperature value
 * @param[in] cpu_critical The specified cpu critical temperature value
 * @param[in] sys_hysteresis The specified sys hysteresis temperature value
 * @param[in] sys_warning The specified sys warning temperature value
 * @param[in] sys_critical The specified sys critical temperature value
 * @param[in] hdd_hysteresis The specified pd hysteresis temperature value
 * @param[in] hdd_warning The specified pd warning temperature value
 * @param[in] hdd_critical The specified pd critical temperature value
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Init_EC_7Level_Temp(IN int enc_id, IN int cpu_hysteresis, IN int cpu_warning, IN int cpu_critical,
                                      IN int sys_hysteresis, IN int sys_warning, IN int sys_critical,
                                      IN int hdd_hysteresis, IN int hdd_warning, IN int hdd_critical);

/**
 * @fn int HM_Set_EC_7Level_Auto(IN int enc_id)
 * @brief Active EC 7-level auto mode
 *
 * This function will active EC 7-level auto mode
 * @param[in] enc_id Enclosure identifier.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_7Level_Auto(IN int enc_id);

/**
 * @fn int int HM_Set_EC_7Level_Custom(IN int enc_id,IN int sys_hysteresis, int sys_stop,
                                                              IN int sys_warning, IN int sys_critical, IN FAN_REGION region)
 * @brief Active EC 7-level auto mode but use custom temperature parameter.
 *
 * This function will active EC 7-level auto mode but use custom temperature parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] sys_hysteresis The specified system hysteresis temperature value, -1 if no change.
 * @param[in] sys_stop The specified system stop temperature value, -1 if no change.
 * @param[in] sys_warning The specified system warning temperature value, -1 if no change.
 * @param[in] sys_critical The specified system critical temperature value, -1 if no change.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_7Level_Custom(IN int enc_id,IN int sys_hysteresis, int sys_stop,
                                        IN int sys_warning, IN int sys_critical, IN FAN_REGION region);

/**
 * @fn int HM_Set_EC_7Level_Manual(IN int enc_id,IN FAN_SPEED speed)
 * @brief Active EC 7-level MANUAL mode
 *
 * This function will active EC 7-level manual mode
 * @param[in] enc_id Enclosure identifier.
 * @param[in] speed The specified EC QFAN speed.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_7Level_Manual(IN int enc_id,IN FAN_SPEED speed);

int HM_Set_Hwtest_State(IN int state);
int pic_sys_set_hwtest_state(IN int state);

/**
 * @fn int HM_Set_EC_TFan_Manual(IN int enc_id, IN FAN_REGION region)
 * @brief Active EC TFAN MANUAL mode
 *
 * This function will active EC TFan manual mode
 * @param[in] enc_id Enclosure identifier.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_TFan_Manual(IN int enc_id, IN FAN_REGION region);

/**
 * @fn int HM_Set_EC_TFan_Auto(IN int enc_id, IN FAN_REGION region)
 * @brief Init EC TFan PWM related parameters and active EC tfan auto mode
 *
 * This function will active EC TFan auto mode but use custom temperature parameters and active EC TFAN manual mode
 * @param[in] enc_id Enclosure identifier.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_TFan_Auto(IN int enc_id, IN FAN_REGION region);

/**
 * @fn int HM_Set_EC_TFan_Custom(IN int enc_id,IN int stop_sys_temp, IN int low_sys_temp, IN int high_sys_temp, IN FAN_REGION region)
 * @brief Active EC Tfan auto mode but use custom temperature parameter.
 *
 * This function will active EC TFan auto mode but use custom temperature parameters.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] stop_sys_temp The specified system stop temperature value, -1 if no change.
 * @param[in] low_sys_temp The specified system low temperature value, -1 if no change.
 * @param[in] high_sys_temp The specified system high temperature value, -1 if no change.
 * @param[in] region The specified fan region, which stands for SysFan or CPU Fan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_TFan_Custom(IN int enc_id,IN int stop_sys_temp, IN int low_sys_temp, IN int high_sys_temp, IN FAN_REGION region);

/**
 * @fn int int HM_Set_EC_TFAN_Parameters(IN int enc_id, IN int temp_critical, IN int temp_hyst_high, IN int temp_hyst_low, IN FAN_REGION region)
 * @brief Set some parameters to EC for the TFAN.
 *
 * This function will set some parameters to EC for the TFAN.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] temp_critical The critical temperature of the specified fan region.
 * @param[in] temp_hyst_high The hysteresis high temperature of the specified fan region.
 * @param[in] temp_hyst_low The hysteresis low temperature of the specified fan region.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_EC_TFAN_Parameters(IN int enc_id, IN int temp_critical, IN int temp_hyst_high, IN int temp_hyst_low, IN FAN_REGION region);

/**
 * @fn int HM_Get_Pic_Ver(IN int enc_id, OUT char *ver, IN unsigned int ver_buf_len)
 * @brief Retrieve the Pic Version.
 *
 * This function will retrieve the Pic Version.
 * @param[in] enclosure identifier.
 * @param[out] pic version will be stored here.
 * @param[in] ver_buf_len is the length of output string buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Get_Pic_Ver(IN int enc_id, OUT char *ver, IN unsigned int ver_buf_len);

/**
 * @fn int HM_Get_Power_Model(IN int enc_id, OUT unsigned char *buf)
 * @brief Retrieve the Power Model.
 *
 * This function will retrieve the Power Model.
 * @param[in] enclosure identifier.
 * @param[out] power model will be stored here.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Get_Power_Model(IN int enc_id, OUT unsigned char *buf);

/**
 * @fn int HM_Get_Warning_Temp(IN int enc_id, OUT unsigned char *buf)
 * @brief Retrieve the Warning Temperature.
 *
 * This function will retrieve the Warning Temperature.
 * @param[in] enclosure identifier.
 * @param[out] buf data will be stored here.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Get_Warning_Temp(IN int enc_id, OUT unsigned char *buf);

/**
 * @fn int HM_Set_Warning_Temp(IN int enc_id, IN unsigned char *buf)
 * @brief Retrieve the Warning Temperature.
 *
 * This function will retrieve the Warning Temperature.
 * @param[in] enclosure identifier.
 * @param[in] buf ASCII code warninig temperature.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HM_Set_Warning_Temp(IN int enc_id, IN char *buf);


/*@}*/  /* end of hardware monitor module */

/**
 * @defgroup LIB_NIC Network Device function
 */

/**
 * @ingroup LIB_NIC Network Devuce function
 */
/*@{*/  /* begin of network device module */

/**
 * @struct  NET_NIC_INFO
 * @brief   The hw information of specific NIC.
 */

#define HAL_QM2_NIC_CONF "/etc/.qm2_nic.conf"
/* QNAP MAC prefix */
#define QNAP_MAC1 "00:08:9b:"
#define QNAP_MAC2 "24:5e:be:"
#define MAX_NIC_LEN 128
#define MAX_ETH_LEN 8
#define QNA_TYPE_LEN 6
#define MODEL_ID_LEN 8
#define ETH_DEV_PATH "/sys/class/net/eth%d/device/"
#define CAT_ETH_DEV "/bin/cat "ETH_DEV_PATH

#define NIC_ISER    32
#define NIC_RDMA    16
#define NIC_ROCEV1  8
#define NIC_ROCEV2  4
#define NIC_IWARP   2
#define NIC_SRIOV   1

typedef struct _NET_NIC_INFO
{
    int link_status;    /*!< Link Status, 0= link down, 1= link up. */
    int max_mtu;        /*!< Max MTU */
    int cur_mtu;        /*!< Current MTU */
    int max_speed;      /*!< Max Link Speed */
    int cur_speed;      /*!< Current Link Speed */
    char nic_model_name[MAX_NIC_LEN];
    char nic_vendor_name[MAX_NIC_LEN];
    int nic_vid;     /* NIC vendor id e.g. intel nic vender=0x8086 */
    int nic_did;
    int nic_subvid;     /* subsystem_vendor e.g. 0x8086 */
    int nic_subdid;     /* subsystem_device e.g. 0x0000 */
    int Is_Qnap;
    /* capability bit 6th to 1st : iSER RDMA RoCEv1 RoCEv2 iWARP SRIOV */
    uint32_t capability;
    char nic_mac_addr[MAX_MAC_ASCII_LEN];
    char nickname[32];
} NET_NIC_INFO;

typedef struct _NIC_PARAM {
    int vendor;
    int device;
    /* capability bit 6th to 1st : iSER RDMA RoCEv1 RoCEv2 iWARP SRIOV */
    uint32_t capability;
} NIC_PARAM;

typedef struct _NIC_VENDOR_MAP {
    int vendor_id;
    char *vendor_name;
} NIC_VENDOR_MAP;

// The follow section for define index of switch port.
#define NET_SW_PORT1        4           /*!< Switch port 1 index */
#define NET_SW_PORT2        3           /*!< Switch port 2 index */
#define NET_SW_PORT3        2           /*!< Switch port 3 index */
#define NET_SW_PORT4        1           /*!< Switch port 4 index */

/**
 * @enum        NIC_TYPE
 * @brief       Network interface type.
 */
typedef enum _NIC_TYPE
{
    NIC_WIRE = 0,       /*!< Wire interface. */
    NIC_WIRELESS,       /*!< Wireless interface. */
    NIC_ALL,            /*!< All kinds of interface. */
} NIC_TYPE;

/**
 * @enum        NIC_SPEED
 * @brief       NIC speed.
 */
typedef enum _NIC_SPEED
{
    NIC_UNKNOWN = 0,    /*!< Unknown speed. */
    NIC_10M,            /*!< 10Mbps. */
    NIC_100M,           /*!< 100Mbps. */
    NIC_1G,             /*!< 1Gbps. */
    NIC_5G,             /*!< 1Gbps. */
    NIC_10G,            /*!< 10Gbps. */
} NIC_SPEED;

/**
 * @enum      PORT_LINK_SPEED
 * @brief     Switch port link speed.
 */
typedef enum _PORT_LINK_SPEED
{
    PORT_LINK_10M   = 0,                /*!< Link speed = 10M */
    PORT_LINK_100M  = 1,                /*!< Link speed = 100M */
    PORT_LINK_1000M = 2                 /*!< Link speed = 1000M */
} PORT_LINK_SPEED;

/**
 * @struct  NET_SW_INFO
 * @brief   The information of switch port.
 */
typedef struct _NET_SW_INFO
{
    int Link_State;                     /*!< Link Status, 0= link status in not okay, 1= link status is okay */
    PORT_LINK_SPEED Link_Speed;         /*!< Link speed */
} NET_SW_INFO;

/**
 * @enum      NET_SW_NIC_CAPABILITY
 * @brief     NIC support capability.
 */
enum _NET_SW_NIC_CAPABILITY
{
    _NIC_NO_CAP  = 0,
    _NIC_ROCE_CAP,
    _NIC_IWARP_CAP
};

/**
 * @fn int Net_Scan(IN void)
 * @brief Scan the attached NIC and fixup the naming sequence.
 *
 * This function will discover the available NIC devices and return the number of them.
 * @retval >= 0 Success, the number of NIC devices.
 * @retval -1 Generic failure.
 */
int Net_Scan(IN void);

/**
 * @fn int Net_NIC_Get_Count(IN NIC_TYPE nic_type)
 * @brief Retrieve the number of NICs for the specified NIC type.
 *
 * This function will retrieve the available NIC devices for the specified type.
 * @retval >= 0 The number of NIC devices.
 */
int Net_NIC_Get_Count(IN NIC_TYPE nic_type);

/**
 * @fn int Net_NIC_Get_Info(IN int port_id, OUT NIC_SPEED* nic_speed)
 * @brief Retrieve the hardware information for the specified NIC.
 *
 * This function will retrieve the hardware information for the specified NIC.
 * @param[in] port_id NIC port identifier.
 * @param[out] nic_infoP A NET_NIC_INFO pointer to accomodate the retrieved NIC info.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int Net_NIC_Get_Info(IN int port_id, OUT NET_NIC_INFO* nic_infoP);

/**
 * @fn int Net_NIC_Get_MAC(IN int port_id, OUT char* mac_addr, IN unsigned int buf_len)
 * @brief Retrieve the MAC address of the specified NIC.
 *
 * This function will retrieve the MAC address of the specified NIC device.
 * @param[in] port_id NIC port identifier.
 * @param[out] mac_addr The retrived MAC address will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int Net_NIC_Get_MAC(IN int port_id, OUT char* mac_addr, IN unsigned int buf_len);

/**
 * int Net_NIC_Set_MAC(IN int port_id, IN char* mac_addr)
 * @brief Set up the MAC address to the specified NIC.
 *
 * This function will set up the MAC address to the specified NIC device.
 * @param[in] port_id NIC port identifier.
 * @param[in] mac_addr The MAC address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int Net_NIC_Set_MAC(IN int port_id, IN char* mac_addr);

/**
 * @fn int Net_NIC_Is_SW_Support(IN int eth_index)
 * @brief Gheck sw support on this NIC.
 *
 * This function will check sw support on this NIC.
 * @param[in] eth_index The index of enthernet port. (base on 1)
 * @retval 1 Support sw on this NIC.
 * @retval 0 Not support sw on this NIC.
 */
int Net_NIC_Is_SW_Support(IN int eth_index);

/**
 * @fn int Net_NIC_Is_QA_Port(IN int port_id)
 * @brief Check if the specified NIC is Quick Access (QA) port.
 *
 * This function will check if the specified NIC is QA port or not.
 * @param[in] port_id The index of port identifer. (base on 1)
 * @retval 1 The specified NIC is QA port.
 * @retval 0 The specified NIC is not QA port.
 */
int Net_NIC_Is_QA_Port(IN int port_id);

/**
 * @fn int Net_NIC_Get_QA_Port_Bitmap(OUT int* bitmapP)
 * @brief Get Quick Access (QA) port bitmap.
 *
 * This function will get QA port bitmap.
 * @param[out] bitmapP QA port bitmap.
 * @retval num Number of QA port.
 */
int Net_NIC_Get_QA_Port_Bitmap(OUT int* bitmapP);

/**
 * @fn int Net_SW_SMI_Read(IN unsigned short addr, OUT unsigned short *rev_Data)
 * @brief Read Data from SMI slave.
 *
 * This function will read data from SMI slave.
 * @param[in] addr Slave address.
 * @param[out] rev_Data Data to be read.
 * @retval 0 Success.
 * @retval -1 No acknowledge when pre-setting.
 * @retval -2 No acknowledge when transmitting address[7:0].
 * @retval -3 No acknowledge when transmitting address[15:8].
 */
int Net_SW_SMI_Read(IN unsigned short addr, OUT unsigned short *rev_Data);

/**
 * @fn int Net_SW_Get_Bitmap(IN int sw_port, OUT int* bitmapP)
 * @brief Get the switch contains several NICs.
 *
 * This function will get the switch contains several NICs.
 * @param[in] sw_port Network switch port.
 * @param[out] bitmapP Switch port bitmap.
 * @retval  0 Found NICs.
 * @retval -1 not found.
 */
int Net_SW_Get_Bitmap(IN int sw_port, OUT int* bitmapP);

/**
 * @fn int Net_MG_Get_Bitmap(OUT int* bitmapP)
 * @brief Get the management port contains several NICs.
 *
 * This function will get the management port.
 * @param[out] bitmapP management port bitmap.
 * @retval  0 Found NICs.
 * @retval -1 not found.
 */
int Net_MG_Get_Bitmap(OUT int* bitmapP);

/**
 * @fn int Net_SW_Get_Num(void)
 * @brief Get total switch port number.
 *
 * This function will get total switch port number.
 * @retval new_sw_max_port total switch port.
 */
int Net_SW_Get_Num(void);

/**
 * @fn int Net_SW_Get_Info(NET_SW_INFO *sw_info, int port_index)
 * @brief Get the switch port information from cached file.
 *
 * This function will get the switch port information from cache file
 * /var/.switch_status.conf.
 * @param[out] sw_info Switch information.
 * @param[out] port_index The port index of switch.
 * @retval >=0 Success.
 * @retval -1 Input Error.
 */
int Net_SW_Get_Info(NET_SW_INFO *sw_info, int port_index);

/**
 * @fn int Net_SW_Scan_Status(void)
 * @brief Scan the switch port information to cache file.
 *
 * This function will get the switch port information and store to cache file
 * /var/.switch_status.conf.
 * @retval 0 Switch Link Down.
 * @retval 1 Switch Link Up.
 * @retval <0 Failed.
 */
int Net_SW_Scan_Status(void);

/**
 * @fn int Net_SW_Get_NIC_ID(void)
 * @brief Get the NIC ID.
 *
 * This function will get the NIC ID.
 * @retval >=0 NIC ID.
 * @retval -1 Failed to read configure file.
 */
int Net_SW_Get_NIC_ID(void);

/**
 * @fn int Net_SW_Get_NIC_Capability(IN int port_id)
 * @brief Get the NIC support capability of port_id .
 *
 * This function will return the NIC capability value of port_id.
 * @param[in] port_id The index of port identifer. (base on 0).
 * @param[out] nic_cap NIC support capability value.
 * @retval 0 This function finished with no error.
 * @retval <0 Failed to read configure file.
 */
int Net_SW_Get_NIC_Capability(int port_id, uint64_t *nic_cap);

/*@}*/  /* end of network device module */


/**
 * @defgroup HAL_PRINTER Printer function
 */

/**
 * @ingroup HAL_PRINTER Printer function
 */
/*@{*/  /* begin of printer module */

/**
 * @fn          typedef int (*PRT_ENUM_CALLBACK) (IN int port_id, IN void* contextP)
 * @brief       Printer enumeration callback function prototype.
 * @param[in]   port_id Printer port identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 */
typedef int (*PRT_ENUM_CALLBACK) (IN int port_id, IN void* contextP);

/**
 * @struct      PRT_INFO
 * @brief       Printer Information.
 */
typedef struct _PRT_INFO {
    int port_id;                        /*!< Global port id assigned by system dependent layer. */
    char prt_sys_id[MAX_SYS_ID_LEN];     /*!< System dependent usb printer identifier when communicating with the system dependent layer. */
    char prt_ctrl_name[MAX_PRT_CTRL_NAME];/*!< System dependent controller name used in system utility. */
    char vendor[MAX_VENDOR_LEN];        /*!< Vendor name. */
    char model[MAX_MODEL_LEN];          /*!< Model name. */
} PRT_INFO;

/**
 * @fn int PRT_Get_Id_By_Sys_Id(IN char* prt_sys_id, OUT int* port_idP)
 * @brief Retrieve the logical printer identifier via system dependent identifier.
 *
 * This function is called by PnP utility to obtain the assigned logical printer identifier from the
 * internal configuration file.
 * @param[in] prt_sys_id Printer system dependent identifier.
 * @param[out] port_idP a logical printer identifier that has been assigned to this printer.
 * @retval 0 Success, the specified printer identifier is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified printer identifier in the
 *         internal configurationturn file.
 */
int PRT_Get_Id_By_Sys_Id(IN char* prt_sys_id, OUT int* port_idP);

/**
 * @fn int PRT_Scan(void)
 * @brief Scan the connected printer and store them in the internal configuration file.
 *
 * This function will remove the original printer setting in the internal
 * configuration file and invoke the system dependent enumerate function to re-generate them.
 * @retval Return the count of the available printers.
 */
int PRT_Scan(void);

/**
 * @fn int PRT_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN PRT_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available printers from the internal configuration file.
 *
 * This function is called by nas utility to obtain all available printer
 * identifiers in the internal configuration file.
 * @param[out] id_ary An identifier array to host the available port identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Printer enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >= 0 Success, return the count of the available printers, if the count
 *         is larger than id_ary_count, the calling utility should allocate more
 *         space for identifier array to obtain all available enclosure
 *         identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int PRT_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
                  IN PRT_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int PRT_Get_Info(IN int port_id, OUT PRT_INFO *prt_infoP)
 * @brief Retrieve the specified printer information.
 *
 * This function is called by nas utility to obtain the specified printer object information.
 * @param[in] port_id Printer port identifier.
 * @param[out] prt_infoP a PRT_INFO structure pointer to host the printer object information.
 * @retval 0 Success, the specified printer object is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified printer object in the
 *         internal configurationturn file.
 */
int PRT_Get_Info(IN int port_id, OUT PRT_INFO *prt_infoP);

/*@}*/  /* end of printer module */

/**
 * @defgroup HAL_MOTHERBOARD Motherboard function
 */
/*@{*/  /* begin of motherboard module */

/**
 * @enum      PCIe_LINK_SPEED
 * @brief     PCIe link speed.
 */
typedef enum _PCIe_LINK_SPEED
{
    PCIe_LINK_NONE = 0,             /*!< No link connect. */
    PCIe_LINK_2_5G = 1,             /*!< 2.5G link connect. */
    PCIe_LINK_5_G = 2,              /*!< 5G link connect. */
    PCIe_LINK_8_G = 3,              /*!< 8G link connect. */
    PCIe_LINK_16_G = 4,              /*!< 16G link connect. */
} PCIe_LINK_SPEED;

/**
 * @struct      MB_TEMP_PROFILE
 * @brief       Enclosure Temperature Profile
 */
typedef struct _MB_TEMP_PROFILE {
    int cpu_shutdown_temp;                  /*!< CPU shutdown Temperature */
    int sys_shutdown_temp;                  /*!< System shutdown Temperature */
    int disk_shutdown_temp;                 /*!< Disk shutdown Temperature */
    int power_shutdown_temp;                /*!< Power shutdown Temperature */
} MB_TEMP_PROFILE;


/**
 * @fn int HAL_MB_Set_Serial_No(IN char* serial_no)
 * @brief Set up NAS serial number.
 *
 * This function will set up NAS serial number.
 * @param[in] serial_no The serial number that will be written to the NAS.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_Serial_No(IN char* serial_no);

/**
 * @fn int HAL_MB_Get_Serial_No(OUT char* serial_no, IN unsigned int buf_len)
 * @brief Retrieve NAS serial number.
 *
 * This function will retrieve NAS serial number.
 * @param[in] serial_no The retrieved serial number.
 * @param[in] buf_len The output buffer length.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Serial_No(OUT char* serial_no, IN unsigned int buf_len);

/**
 * @struct      MB_VQTS_HW_INFO
 * @brief       The structure of vQTS hardware information retrieved from hypervisor.
 */
typedef struct _MB_VQTS_HW_INFO {
    char host_model[16];          /*!< Host model name, like "TS-677". */
    char host_fw_model[16];       /*!< Host firmware model name, like "TS-X77". */
    char host_fw_build_day[16];   /*!< Host firmware build day, like "20171127". */
    char host_fw_build_ver[16];   /*!< Host firmware build version, like "4.3.4". */
    char host_fw_build_num[16];   /*!< Host firmware build number, like "0125". */
    char host_mac_addr[32];       /*!< The mac address of eth0 on host. */
    char vqts_sn[16];             /*!< vQTS serial number generated by host. */
    char vqts_uuid[64];           /*!< vQTS UUID generated by host. */
    char system_manufacturer[32]; /*!< extended informantion for vQTS cloud. */
    char system_serial_number[64];/*!< extended informantion for vQTS cloud. */
    char system_uuid[64];         /*!< extended informantion for vQTS cloud. */
} MB_VQTS_HW_INFO;

/**
 * @fn int HAL_MB_Get_vQTS_HW_Info(MB_VQTS_HW_INFO *vqts_hw_infoP)
 * @brief Retrieve vQTS hardware information from hypervisor.
 *
 * This function will retrieve vQTS hardware information from hypervisor.
 * It will also set and check UUID key, and will call penalty function if UUID key not match.
 * @param[in/out] vqts_hw_infoP The pointer of the vqts_hw_info.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_vQTS_HW_Info(MB_VQTS_HW_INFO *vqts_hw_infoP);

/**
 * @fn int HAL_MB_Is_vQTS(void)
 * @brief Check this QTS is virtual-QTS or not.
 *
 * This function will check this QTS is virtual-QTS or not.
 * @retval 1 vQTS.
 * @retval 0 not vQTS.
 */
int HAL_MB_Is_vQTS(void);

/**
 * @fn int HAL_MB_vQTS_Penalty(char *app_name, char *reason, int err_code)
 * @brief Retrieve vQTS hardware information from hypervisor.
 *
 * This function will retrieve vQTS hardware information from hypervisor.
 * @param[in] app_name The name of the app calling this function.
 * @param[in] reason A description about the penalty reason.
 * @param[in] err_code An integer indicate the error number.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_vQTS_Penalty(char *app_name, char *reason, int err_code);


/**
 * @fn int HAL_MB_Get_PCIe_Slot_Count(void)
 * @brief Retrieve the number of PCIe slots for this NAS motherboard.
 *
 * This function will retrieve the number of PCIe slots.
 * @retval >= 0 Success, return the count of the available PCIe slots.
 */
int HAL_MB_Get_PCIe_Slot_Count(void);

/**
 * @fn int HAL_MB_Get_PCIe_Info(IN int port, OUT PCIe_LINK_SPEED* link_speedP, OUT int* link_widthP)
 * @brief Retrieve the link speed of the specified PCIe slot.
 *
 * This function will retrieve the link speed of the specified PCIe slot.
 * @param[in] port The PCIe slot number (0 based).
 * @param[out] link_speedP The output PCIe link speed.
 * @param[out] link_widthP The output PCIe link width.
 * @param[out] max_link_speedP The output PCIe max link speed.
 * @param[out] max_link_widthP The output PCIe max link width.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_PCIe_Info(IN int port, OUT PCIe_LINK_SPEED* link_speedP, OUT int* link_widthP, OUT PCIe_LINK_SPEED* max_link_speedP, OUT int* max_link_widthP);

/**
 * @fn int HAL_MB_Get_Device_Link_Capability(int port, PCIe_LINK_SPEED *link_speedP, int * link_widthP);
 * @brief Retrieve the link speed capability of the specified Device.
 *
 * This function will retrieve the link speed capability of the specified device.
 * @param[in] port The PCIe slot number (0 based).
 * @param[out] link_speedP The output device link speed.
 * @param[out] link_widthP The output device link width.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Device_Link_Capability(int port, PCIe_LINK_SPEED *link_speedP, int* link_widthP);

/**
 * @fn int HAL_MB_Get_PCIe_Slot_Num(IN int enc_id,OUT int *slot_num)
 * @brief Retrieve the slot number on motherboard for PCIe/QM2 enclosuare.
 *
 * This function will retrieve slot number in motherboard for PCIe/QM2 enclosuare.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] slot_num PCIe slot number
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_PCIe_Slot_Num(IN int enc_id,OUT int *slot_num);

/**
 * @fn int HAL_MB_Get_NIC_Count(void)
 * @brief Retrieve the number of NICs for this NAS motherboard.
 *
 * This function will retrieve the number of NIC ports.
 * @retval >= 0 Success, return the count of the available NIC ports.
 */
int HAL_MB_Get_NIC_Count(void);

/**
 * @fn int HAL_MB_Get_NIC_MAC(IN int port, OUT char* mac_addr, IN unsigned int buf_len)
 * @brief Retrieve the MAC address of the specified built-in NIC.
 *
 * This function will retrieve the MAC address of the specified built-in NIC device.
 * @param[in] port NIC port identifier (0 based).
 * @param[out] mac_addr The retrived MAC address will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_NIC_MAC(IN int port, OUT char* mac_addr, IN unsigned int buf_len);

/**
 * int HAL_MB_Set_NIC_MAC(IN int port, IN char* mac_addr)
 * @brief Set up the MAC address to the specified built-in NIC.
 *
 * This function will set up the MAC address to the specified built-in NIC device.
 * @param[in] port NIC port identifier (0 based).
 * @param[in] mac_addr The MAC address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_NIC_MAC(IN int port, IN char* mac_addr);

/**
 * @fn int HAL_MB_Get_NIC_Speed(IN int port_id, OUT NIC_SPEED* nic_speedP)
 * @brief Retrieve the link speed for the specified built-in NIC.
 *
 * This function will retrieve the link speed for the specified built-in NIC device.
 * @param[in] port_id NIC port identifier (0 based).
 * @param[out] nic_speedP A NIC_SPEED pointer to accomodate the retrieved link speed.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_NIC_Speed(IN int port, OUT NIC_SPEED* nic_speedP);

/**
 * @fn int HAL_MB_Is_NIC_MAC_Locked(IN int port)
 * @brief Check if MAC address can update or not.
 *
 * This function will check NIC MAC address can update or not.
 * @retval 1, the specified NIC MAC is locked.
 * @retval 0, the specified NIC MAC is unlocked.
 */
int HAL_MB_Is_NIC_MAC_Locked(IN int port);

/**
 * @fn int HAL_MB_Get_PR_Support()
 * @brief Retrieve the bitmap of power recovery mode for this NAS motherboard.
 *
 * This function will retrieve the bitmap of power recovery mode.
 * @retval pr_bitmapP Power recover support mode.
 */
int HAL_MB_Get_PR_Support();

/**
 * @fn int HAL_MB_Is_IPMI_Support(void)
 * @brief Check IPMI feature is support or not.
 *
 * This function is called to inquiry IPMI feature is support or not.
 * @retval 1 IPMI support.
 * @retval 0 IPMI not support.
 */
int HAL_MB_Is_IPMI_Support(void);

/**
 * @fn int HAL_MB_Get_IPMI_MAC(OUT char* mac_addr, IN unsigned int buf_len)
 * @brief Retrieve the IPMI MAC address.
 *
 * This function will retrieve the IPMI MAC address .
 * @param[out] mac_addr The retrived IPMI MAC address will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_IPMI_MAC(OUT char* mac_addr, IN unsigned int buf_len);

/**
 * @fn int HAL_MB_Set_IPMI_MAC(IN char* mac_addr)
 * @brief Set up the IPMI MAC address.
 *
 * This function will set up the IPMI MAC address.
 * @param[in] mac_addr The MAC address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_IPMI_MAC(IN char* mac_addr);

/**
 * @fn int HAL_MB_Get_USB_Count(void)
 * @brief Retrieve the number of USB ports for this NAS motherboard.
 *
 * This function will retrieve the number of USB ports.
 * @retval >= 0 Success, return the count of the available USB ports.
 */
int HAL_MB_Get_USB_Count(void);

/**
 * @fn int HAL_MB_Get_Ext_USB_Count(void)
 * @brief Retrieve the number of USB ports on addon cards for this NAS motherboard.
 *
 * This function will retrieve the number of USB ports.
 * @retval >= 0 Success, return the count of the available USB ports.
 */
int HAL_MB_Get_Ext_USB_Count(void);

/**
 * @fn int MB_SE_Enum_By_Root_USB_Port(IN int port_id, OUT int id_ary[], IN unsigned int id_ary_count,
 *                                     IN SE_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate USB enclosures which are located in the specified Root USB port.
 *
 * This function is called by factory test utility to obtain all available USB enclosures
 * located in the specified Root USB port.
 * @param[in] port_id Root USB port identifier. (0 based)
 * @param[out] id_ary An identifier array to host the available enclosure identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback Enclosure enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >= 0 Success, return the count of the available USB enclosures, if the count
 *         is larger than id_ary_count, the calling utility should allocate more
 *         space for identifier array to obtain all available enclosure
 *         identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int MB_SE_Enum_By_Root_USB_Port(IN int port_id, OUT int id_ary[], IN unsigned int id_ary_count,
                                IN SE_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int HAL_MB_Get_USB_Sys_Name(IN int port, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified built-in USB port of root enclosure.
 *
 * This function is called by system utility to obtain the system dependent name for specified USB port.
 * @param[in] port Physical port id assigned by system dependent layer (0 based).
 * @retval 1 TRUE, the specified USB port is USB 3.0.
 * @retval 0 FALSE, the specified USB port is not USB 3.0.
 */
int HAL_MB_Is_USB_Port_3_0(IN int port);

/**
 * @fn int HAL_MB_Get_USB_Sys_Name(IN int port, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified built-in USB port of root enclosure.
 *
 * This function is called by system utility to obtain the system dependent name for specified USB port.
 * @param[in] port Physical port id assigned by system dependent layer (0 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified USB port is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified USB port in the
 *         internal configurationturn file.
 */
int HAL_MB_Get_USB_Sys_Name(IN int port, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int HAL_MB_Get_eSATA_Count(void)
 * @brief Retrieve the number of eSATA ports for this NAS motherboard.
 *
 * This function will retrieve the number of eSATA ports.
 * @retval >= 0 Success, return the count of the available eSATA ports.
 */
int HAL_MB_Get_eSATA_Count(void);

/**
 * @fn int HAL_MB_Get_ODD_Count(void)
 * @brief Retrieve the number of ODD ports for this NAS motherboard.
 *
 * This function will retrieve the number of ODD ports.
 * @retval >= 0 Success, return the count of the available ODD ports.
 */
int HAL_MB_Get_ODD_Count(void);

/**
 * @fn int HAL_MB_Get_eSATA_Sys_Name(IN int port, OUT char* sys_name, IN unsigned int buf_len)
 * @brief Retrieve the system name for the specified built-in eSATA port of root enclosure.
 *
 * This function is called by system utility to obtain the system dependent name for specified eSATA port.
 * @param[in] port Physical port id assigned by system dependent layer (0 based).
 * @param[out] sys_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sys_name.
 * @retval 0 Success, the specified eSATA port is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified USB port in the
 *         internal configurationturn file.
 */
int HAL_MB_Get_eSATA_Sys_Name(IN int port, OUT char* sys_name, IN unsigned int buf_len);

/**
 * @fn int HAL_MB_Get_Display_Name(char *model, int buf_size)
 * @brief Retrieve the display model name.
 *
 * This function is called by hwtest to obtain the display model name.
 * @param[out] model model name string.
 * @param[in] buf_size string size.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Display_Name(OUT char *model, IN int buf_size);

/**
 * @fn int HAL_MB_ECC_Mem_Installed(void)
 * @brief Retrieve the display model name.
 *
 * This function is called by hwtest to obtain the status of ecc memory.
 * @retval True ECC memory is detected.
 * @retval False ECC memory is not installed.
 */
int HAL_MB_ECC_Mem_Installed(void);

/**
 * @fn int HAL_MB_Get_CPU_Info(OUT char *cpu_info, IN int max_length)
 * @brief Retrieve the CPU information.
 *
 * This function is called by hwtest to obtain the CPU information.
 * @param[out] cpu_info information string.
 * @param[in] max_length string size.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_CPU_Info(OUT char *cpu_info, IN int max_length);

/**
 * @fn int HAL_MB_Get_CPU_Info_By_Index(IN int cpu_index, OUT char *cpu_info, IN int max_length)
 * @brief Retrieve the CPU information by cpu_index
 *
 * This function is called by hwtest to obtain the CPU information by cpu_index
 * @param[in] cpu_index index(0 based).
 * @param[out] cpu_info information string.
 * @param[in] max_length string size.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_CPU_Info_By_Index(IN int cpu_index, OUT char *cpu_info, IN int max_length);

/**
 * @fn int HAL_MB_Get_CPU_Speed(OUT int *cpu_running_MHz)
 * @brief Retrieve the CPU Speed in MHz unit.
 *
 * This function is called by hwtest to obtain the CPU speed information.
 * @param[out] cpu_running_MHz cpu speed value.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_CPU_Speed(OUT int *cpu_running_MHz);

/**
 * @fn int HAL_MB_Get_CPU_Speed_By_Index(OUT int *cpu_running_MHz)
 * @brief Retrieve the CPU Speed in MHz unit by cpu_index
 *
 * This function is called by hwtest to obtain the CPU speed information by cpu_index
 * @param[in] cpu_index index(0 based).
 * @param[out] cpu_running_MHz cpu speed value.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_CPU_Speed_By_Index(IN int cpu_index, OUT int *cpu_running_MHz);

/**
 * @fn int HAL_MB_Get_Internal_SAS_Addr(IN int internal_id, OUT char* sas_addr, IN int buf_len)
 * @brief Retrieve the SAS address of the host controller in motherboard.
 *
 * This function is called by hwtest to retrieve the SAS address of the SAS host controller.
 * @param[in] internal_id The identifer that SAS host controller resided on(1 based)
 * @param[out] sas_addr SAS address returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sas_addr.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Internal_SAS_Addr(IN int internal_id,OUT char* sas_addr, IN int buf_len);

/**
 * @fn int HAL_MB_Set_Internal_SAS_Addr(IN int internal_id, IN char* sas_addr)
 * @brief Set up the SAS address of the host controller in motherboarcd.
 *
 * This function will set up the SAS address of the SAS host controller.
 * @param[in] internal_id The identifer that SAS host controller resided on(1 based)
 * @param[in] sas_addr The SAS address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_Internal_SAS_Addr(IN int internal_id, IN char* sas_addr);

/**
 * @fn int HAL_MB_Get_Expander_SAS_Addr(OUT char* sas_addr, IN int buf_len)
 * @brief Retrieve the SAS address of the SAS expander in motherboard.
 *
 * This function is called by hwtest to retrieve the SAS address of the SAS expander.
 * @param[out] sas_addr SAS address returned by system dependent layer.
 * @param[in] buf_len The buffer count for the sas_addr.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Expander_SAS_Addr(OUT char* sas_addr, IN int buf_len);

/**
 * @fn int HAL_MB_Set_Expander_SAS_Addr(IN char* sas_addr)
 * @brief Set up the SAS address of the SAS expander in the motherboard.
 *
 * This function will set up the SAS address of the SAS expander.
 * @param[in] sas_addr The SAS address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_Expander_SAS_Addr(IN char* sas_addr);

/**
 * @fn int HAL_MB_Get_External_SAS_Info(OUT int *connectorsP, OUT int* portsP)
 * @brief Retrieve the external SAS connector and port count.
 *
 * This function is called by hwtest utility to obtain the external SAS connector and port count.
 * @param[out] connectorsP Connector count returned by system dependent layer.
 * @param[out] portsP Port count for each connector.
 * @retval 0 Success, the external SAS information is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the external SAS information.
 */
int HAL_MB_Get_External_SAS_Info(OUT int *connectorsP, OUT int* portsP);

/**
 * @fn int HAL_MB_Get_External_SAS_PD_Sys_Name(IN int connector, IN int port, OUT char* pd_name, IN int buf_len)
 * @brief Retrieve the system name for the specified external SAS disk.
 *
 * This function is called by hwtest utility to obtain the system dependent name for specified physical disk.
 * @param[in] connector  Physical connector id assigned by system dependent layer (0 based).
 * @param[in] port Physical port id assigned by system dependent layer (0 based).
 * @param[out] pd_name System dependent name returned by system dependent layer.
 * @param[in] buf_len The buffer count for the pd_name.
 * @retval 0 Success, the specified external SAS disk is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the system name for specified external SAS disk.
 */
int HAL_MB_Get_External_SAS_PD_Sys_Name(IN int connector, IN int port, OUT char* pd_name, IN int buf_len);

/**
 * @fn int HAL_MB_Get_Internal_SAS_HBA_Count(void)
 * @brief Retrieve the number of Interanl SAS HBA for this NAS motherboard.
 *
 * This function will retrieve the number of Interanl SAS HBA for this NAS motherboard.
 * @retval >= 0 Success, return the count of the available Internal SAS HBA.
 */
int HAL_MB_Get_Internal_SAS_HBA_Count(void);

/**
 * @fn int HAL_MB_Get_Internal_SAS_HBA_FW_Version(IN int internal_id, OUT char* versionP, IN int buf_len)
 * @brief Retrieve each internal SAS chip firmware version
 *
 * This function is called by hwtest utility to obtain each internal SAS firmware version.
 * @param[in] internal_id The identifer that SAS host controller resided on(1 based)
 * @param[out] versionP HBA firmware version returned by system dependent layer.
 * @param[in] buf_len The buffer count for versionP.
 * @retval 0 Success, the SAS HBA firmware version is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the SAS HBA firmware version.
 */
int HAL_MB_Get_Internal_SAS_HBA_FW_Version(IN int internal_id, OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_PCIE_SAS_HBA_FW_Version(IN int pcie_slot, OUT char* versionP, IN int buf_len)
 * @brief Retrieve the SAS HBA firmware version on PCIe slot
 *
 * This function is called by hwtest utility to obtain the SAS HBA firmware version on PCIE slot
 * @param[in] pcie_slot The PCIe slot id (1 based) that SAS host controller resided on,
 * @param[out] versionP HBA firmware version returned by system dependent layer.
 * @param[in] buf_len The buffer count for versionP.
 * @retval 0 Success, the SAS HBA firmware version is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the SAS HBA firmware version.
 */
int HAL_MB_Get_PCIE_SAS_HBA_FW_Version(IN int pcie_slot, OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_Internal_SAS_HBA_BIOS_Version(IN int internal_id, OUT char* versionP, IN int buf_len)
 * @brief Retrieve each internal SAS chip bios version
 *
 * This function is called by hwtest utility to obtain each internal SAS bios version.
 * @param[in] internal_id The identifer that SAS host controller resided on(1 based)
 * @param[out] versionP HBA bios version returned by system dependent layer.
 * @param[in] buf_len The buffer count for versionP.
 * @retval 0 Success, the SAS HBA bios version is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the SAS HBA bios version.
 */
int HAL_MB_Get_Internal_SAS_HBA_BIOS_Version(IN int internal_id, OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_PCIE_SAS_HBA_BIOS_Version(IN int pcie_slot, OUT char* versionP, IN int buf_len)
 * @brief Retrieve the SAS HBA bios version on PCIe slot
 *
 * This function is called by hwtest utility to obtain the SAS HBA bios version on PCIE slot
 * @param[in] pcie_slot The PCIe slot id (1 based) that SAS host controller resided on,
 * @param[out] versionP HBA bios version returned by system dependent layer.
 * @param[in] buf_len The buffer count for versionP.
 * @retval 0 Success, the SAS HBA bios version is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the SAS HBA bios version.
 */
int HAL_MB_Get_PCIE_SAS_HBA_BIOS_Version(IN int pcie_slot, OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_SAS_Expander_FW_Version(OUT char* versionP, IN int buf_len)
 * @brief Retrieve the SAS Expander firmware version.
 *
 * This function is called by hwtest utility to obtain the SAS Expander firmware version.
 * @param[out] versionP Expander firmware version returned by system dependent layer.
 * @param[in] buf_len The buffer count for versionP.
 * @retval 0 Success, the SAS Expander firmware version is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the SAS Expander firmware version.
 */
int HAL_MB_Get_SAS_Expander_FW_Version(OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Upgrade_SAS_HBA_FW(IN int slot_id, IN char* img_path, IN int sas_version)
 * @brief Upgrade the onboard SAS HBA firmware.
 *
 * This function will upgrade the onboard SAS HBA firmware and return the upgraded
 * firmware version after upgrading successfully.
 * @param[in] slot_id The PCIe slot id (0 based) that SAS host controller resided on,
 * 0 represents on board SAS host controller.
 * @param[in] img_path The original single image file path.
 * @param[in] sas_version The sas version of the HBA device.
 * @retval 0 Success, The onboard SAS HBA has been upgraded to the specified binary image.
 * @retval < 0 Generic failure, Fail to update the specified binary image.
 */
int HAL_MB_Upgrade_SAS_HBA_FW(IN int slot_id, IN char* img_path, IN int sas_version);

/**
 * @fn int HAL_MB_Upgrade_SAS_BIOS(IN int slot_id, IN char* img_path, IN int sas_version)
 * @brief Upgrade the onboard SAS HBA firmware.
 *
 * This function will upgrade the onboard SAS Bios and return the upgraded
 * firmware version after upgrading successfully.
 * @param[in] slot_id The PCIe slot id (0 based) that SAS host controller resided on,
 * 0 represents on board SAS host controller.
 * @param[in] img_path The original single image file path.
 * @param[in] sas_version The sas version of the HBA device.
 * @retval 0 Success, The onboard SAS HBA has been upgraded to the specified binary image.
 * @retval < 0 Generic failure, Fail to update the specified binary image.
 */
int HAL_MB_Upgrade_SAS_BIOS(IN int slot_id, IN char* img_path, IN int sas_version);

/**
 * @fn int HAL_MB_Upgrade_SAS_Expander_FW(IN char* img_path, OUT char* new_versionP, IN int buf_len)
 * @brief Upgrade the onboard SAS expander firmware.
 *
 * This function will upgrade the onboard SAS expander firmware and return the upgraded
 * firmware version after upgrading successfully.
 * @param[in] img_path The original single image file path.
 * @param[out] new_versionP The image version after upgarding successfully.
 * @param[in] buf_len The buffer ocunt for new_versionP.
 * @retval 0 Success, The onboard SAS expander has been upgraded to the specified binary image.
 * @retval < 0 Generic failure, Fail to update the specified binary image.
 */
int HAL_MB_Upgrade_SAS_Expander_FW(IN char* img_path, OUT char* new_versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_Temp_Profile(OUT MB_TEMP_PROFILE* profileP);
 * @brief Get Enclosure temperature profile.
 *
 * This function will get temperature limitation from /etc/hal_util.conf
 * @param[out] profileP The temperature profile structure.
 * @retval 0 Success.
 * @retval < 0 Generic failure.
 */
int HAL_MB_Get_Temp_Profile(OUT MB_TEMP_PROFILE* profileP);

//  SWChen 20131213
/**
 * @fn  int HAL_MB_Get_SS_Profile(OUT int* max_channelP, OUT int* free_channelP)
 * @brief Get Surveillance Station profile.
 *
 * @param[out] max_channelP The pointer to store the returned maximum channels.
 * @param[out] free_channelP The pointer of store the returned free channels.
 *
 * @retval 0 Success, the Surveillance Station profile is inquired successfully.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_SS_Profile(OUT int* max_channelP, OUT int* free_channelP);


/**
 * @enum        MEM_TYPE
 * @brief       MEM interface type.
 */
#define MAX_MANUFACTURER		(128)
typedef enum _MEM_TYPE {
	DDR2_SDRAM = 8,
	DDR3_SDRAM = 11,
	DDR4_SDRAM = 12,
	MEM_NO_SUPPORT,
} MEM_TYPE;

typedef enum _MEM_MODULE_TYPE {
    UNDEF = 0,
    RDIMM,
    UDIMM,
    SO_DIMM,
    MICRO_DIMM,
    MINI_RDIMM,
    MINI_UDIMM,
    MEM_MODULE_TYPE_UNKNOW,
} MEM_MODULE_TYPE;

typedef enum _MEM_SPEED {
	MEM_400 = 0,
	MEM_533,
	MEM_800,
	MEM_1066,
	MEM_1333,
	MEM_1600,
	MEM_1866,
	MEM_2133,
	MEM_2400,
	MEM_2666,
	MEM_3200,
	MEM_UNKNOW_SPEED,
} MEM_SPEED;

typedef enum _MEM_ERR_CODE {
    WELL = 0,
    ACCESS_SPD_FAILED,
    SPD_CRC_ERROR,
    DDR_TYPE_NO_SUPPORT,
    DECODE_DDR2_FAILED,
    DECODE_DDR3_FAILED,
    DECODE_DDR4_FAILED,
} MEM_ERR_CODE;

typedef struct _QNAP_MEM_INFO {
    int valid;
    int channel_id;                        /*!< Channel number. */
    int slot_id;                             /*!< Slot (DIMM) number. */
    int slot_addr;             /*!< i2c address for the specified slot (DIMM). */
    MEM_TYPE type;                    /*!< Fundamental memory type. */
    MEM_SPEED speed;                    /*!< Maximum speed support. */
    MEM_MODULE_TYPE module_type;        /*!< Memory dimm module type. */
    int ecc;
    int size;                             /*!< Memory dimm size in MB. */
    char manufaturer[MAX_MANUFACTURER];    /*!< Memory dimm manufacturer. */
    int err_code;              /*!< Memory error code. */
} QNAP_MEM_INFO;

/**
 * @fn int HAL_MB_Mem_Is_Info_Available()
 * @brief check the capability to get memory information
 *
 * This function is called by nas utility to determine if the detail memory information report is available.
 * @retval 1 TRUE, the detail memory information report is available.
 * @retval 0 FALSE, the detail memory information report is not available.
 */
int HAL_MB_Mem_Is_Info_Available();

/**
 * @fn int HAL_MB_Mem_Get_Channels()
 * @brief get the maximum memory channel number
 *
 * This function is called by nas utility to get the maximum memory channel number.
 * @retval >=1 Success, the maximum memory channel number is returned.
 * @retval -1 Generic failure, the detail memory information report is not available.
 */
int HAL_MB_Mem_Get_Channels();

/**
 * @fn int HAL_MB_Get_EC_FW_Version(OUT char* versionP, IN int buf_len)
 * @brief Retrieve the embedded controller firmware version.
 *
 * This function will Retrieve the embedded controller firmware version.
 * @param[out] versionP Character of EC firmware version.
 * @param[in] buf_len Maximum string length.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_EC_FW_Version(OUT char* versionP, IN int buf_len);

/**
 * @fn int HAL_MB_Get_BIOS_Version(OUT char* buf, IN int buf_len);
 * @brief Retrieve the BIOS version.
 *
 * This function will Retrieve the BIOS version.
 * @param[out] buf Character array of BIOS version.
 * @param[in] buf_len Maximum string length.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_BIOS_Version(OUT char* buf, IN int buf_len);

/**
 ** @fn int HAL_MB_Get_UBOOT_Version(OUT char* buf, IN int buf_len);
 ** @brief Retrieve the UBOOT version.
 **
 ** This function will Retrieve the UBOOT version.
 ** @param[out] buf Character array of UBOOT version.
 ** @param[in] buf_len Maximum string length.
 ** @retval 0 Success.
 ** @retval -1 Generic failure.
 **/
int HAL_MB_Get_UBOOT_Version(OUT char* buf, IN int buf_len);

/**
 * @fn int HAL_MB_Mem_Get_Slots()
 * @brief get the maximum memory slot number
 *
 * This function is called by nas utility to get the maximum memory slot number.
 * @retval >=1 Success, the maximum memory slot number is returned.
 * @retval -1 Generic failure, the detail memory information report is not available.
 */
int HAL_MB_Mem_Get_Slots();

/**
 * @fn int HAL_MB_Mem_Size(OUT int* size)
 * @brief Retrieve the memory size in MB.
 *
 * This function will Retrieve the memory size in MB.
 * @param[out] Retrieve the memory size in MB.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Mem_Size(OUT int* size);

/**
 * @fn int HAL_MB_Mem_Get_Info(IN int slot_id, OUT QNAP_MEM_INFO* mem_infoP)
 * @brief inquiry the specified memory dimm information
 *
 * This function is called by nas utility to inquiry the specified memory dimm information.
 * @param[in] slot_id Specified the memory slot (DIMM) identifier (1 based).
 * @param[out] mem_infoP a MEM_INFO structure pointer to host the detail memory information.
 * @retval 0 Success, return the memory dimm info. for the specified slot id.
 * @retval -1 Generic failure, fail to retrieve the specified memory dimm information.
 */
int HAL_MB_Mem_Get_Info(IN int slot_id, OUT QNAP_MEM_INFO* mem_infoP);

/**
 * @fn int HAL_MB_Is_PCIe_WLan_Installed(void)
 * @brief inquiry pcie wireless installed or not.
 *
 * This function is called to inquiry pcie wireless lan is installed or not.
 * @retval 1 PCIe_Wireless LAN Installed.
 * @retval 0 PCIe_Wireless LAN not installed.
 */
int HAL_MB_Is_PCIe_WLan_Installed(void);

/**
 * @fn int HAL_MB_Is_TR_Enc_Support(void)
 * @brief Check if TR enclosures were supported in this unit or not.
 *
 * This function is called to inquiry if TR enclosure was supported in this unit or not.
 * @retval 1 TR enclosure is supported.
 * @retval 0 TR enclosure is not supported.
 */
int HAL_MB_Is_TR_Enc_Support(void);

/**
 * @fn int HAL_MB_Is_TBT_Support(void)
 * @brief Check thunderbolt feature is support or not.
 *
 * This function is called to inquiry thunderbolt feature is support or not.
 * @retval 1 TBT support.
 * @retval 0 TBT not support.
 */
int HAL_MB_Is_TBT_Support(void);

/**
 * @fn int HAL_MB_Get_TBT_Port_Count(void)
 * @brief Retrieve the number of thunderbolt ports for this NAS.
 *
 * This function will retrieve the number of thunderbolt ports.
 * @retval >= 0 Success, return the count of the available buttons.
 */
int HAL_MB_Get_TBT_Port_Count(void);

/**
 * @fn int HAL_MB_TBT_Port_Rename(IN char *old_name, OUT char *new_name)
 * @brief Rename the thunderbolt nic if needed.
 *
 * This function will rename the thunderbolt nic if needed.
 * @param[in] old_name Original name of the thunderbolt nic.
 * @param[out] new_name New name of the thunderbolt nic.
 * @retval 1 Rename nic successfully.
 * @retval 0 Do not need to rename nic.
 * @retval -1 Generic failure.
 */
int HAL_MB_TBT_Port_Rename(IN char *old_name, OUT char *new_name);

/**
 * @fn int HAL_MB_TBT_Port_Get_HBA_Version(IN char *tbtnet_name)
 * @brief This function is used to get the hba version of the specified thunderbolt port name
 * 
 * @param[in] tbtnet_name Port name of the thunderbolt nic port.
 * @retval 2 This port is generated from a thunderbolt2 HBA.
 * @retval 3 This port is generated from a thunderbolt3 HBA.
 * @retval -1 Generic failure.
 */
int HAL_MB_TBT_Port_Get_HBA_Version(IN char *tbtnet_name);

/**
 * @fn int HAL_MB_TBT_Port_Get_NVM_Version(IN char *tbtnet_name, OUT char *nvm_version, IN int ver_len)
 * @brief This function is used to get the hba version of the specified thunderbolt port name
 * 
 * @param[in] tbtnet_name Port name of the thunderbolt nic port.
 * @param[out] nvm_version buffer for storing the nvm version of the thunderbolt nic port.
 * @param[in] ver_len the buffer lenght of the specified nvm_version.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_TBT_Port_Get_NVM_Version(IN char *tbtnet_name, OUT char *nvm_version, IN int ver_len);

/**
 * @fn int HAL_MB_Is_T2E_Support(void)
 * @brief Check thunderbolt T2E feature is support or not.
 *
 * This function is called to inquiry thunderbolt T2E feature is support or not.
 * @retval 1 TBT T2E support.
 * @retval 0 TBT T2E not support.
 */
int HAL_MB_Is_T2E_Support(void);

/**
 * @fn int HAL_MB_Is_HDMI_Support(void)
 * @brief Check HDMI feature is support or not.
 *
 * This function is called to inquiry HDMI feature is support or not.
 * @retval 1 HDMI support.
 * @retval 0 HDMI not support.
 */
int HAL_MB_Is_HDMI_Support(void);

/**
 * @fn int HAL_MB_Is_BBU_Support(void)
 * @brief Check BBU feature is support or not.
 *
 * This function is called to inquiry BBU feature is support or not.
 * @retval 1 BBU support.
 * @retval 0 BBU not support.
 */
int HAL_MB_Is_BBU_Support(void);

/**
 * @fn int HAL_MB_Is_Factory_Recovery_Boot(void)
 * @brief Check system is factory recovery boot.
 *
 * This function is called to inquiry system is factory recovery boot or not.
 * @retval 1 Factory recovery boot.
 * @retval 0 Normal boot.
 */
int HAL_MB_Is_Factory_Recovery_Boot(void);

/**
 * @fn int HAL_MB_Is_Factory_Mode(void)
 * @brief Check system is factory mode or not.
 *
 * This function will check system is factory mode or not.
 * @retval 1 Factory mode.
 * @retval 0 Not factory mode.
 */
int HAL_MB_Is_Factory_Mode(void);

/**
 * @fn int HAL_MB_Is_Voice_Alert_Support(void)
 * @brief Check if the voice alert feature is support or not.
 *
 * This function is called to inquiry if the voice alert feature is support or not.
 * @retval 1 Voice alert support.
 * @retval 0 Voice alert not support.
 */
int HAL_MB_Is_Voice_Alert_Support(void);

/**
 * @fn int HAL_MB_Is_Dmidecode_Support(void)
 * @brief Check if the dmidecode is supported or not.
 *
 * This function is called to inquiry if the dmidecode is supported or not.
 * @retval 1 Dmidecode support.
 * @retval 0 Dmidecode not support.
 */
int HAL_MB_Is_Dmidecode_Support(void);

/**
 * @fn int HAL_MB_Is_EUP_Support(void)
 * @brief Check EUP feature is support or not.
 *
 * This function is called to inquiry EUP feature is support or not.
 * @retval 1 EUP support.
 * @retval 0 EUP not support.
 */
int HAL_MB_Is_EUP_Support(void);

/**
 * @fn int HAL_MB_Is_ENV_TS_Support(OUT int* count)
 * @brief Check env thermal sensor support or not.
 *
 * This function is called to checn env thermal sensor support or not,
 * and return counts if support.
 * @param[out] count thermal sensor counts.
 * @retval 1 ENV_TS support.
 * @retval 0 ENV_TS not support.
 */
int HAL_MB_Is_ENV_TS_Support(OUT int* count);

/**
 * @fn int HAL_MB_Is_EC_Support(void)
 * @brief Check EC feature is support or not.
 *
 * This function is called to inquiry EC feature is support or not.
 * @retval 1 EC support.
 * @retval 0 EC not support.
 */
int HAL_MB_Is_EC_Support(void);

/**
 * @fn int HAL_MB_Is_QA_Port_Support(void)
 * @brief Check quick access port support or not.
 *
 * This function is called to check quick access port support or not.
 * @retval 1 Quick access port support.
 * @retval 0 Quick access port not support.
 */
int HAL_MB_Is_QA_Port_Support(void);

/**
 * @fn int HAL_MB_Is_DA_Port_Support(void)
 * @brief Check DA Port feature is support or not.
 *
 * This function is called to inquiry DA Port feature is support or not.
 * @retval 1 DA Port support.
 * @retval 0 DA Port not support.
 */
int HAL_MB_Is_DA_Port_Support(void);

/**
 * @fn int HAL_MB_Is_Switch_Support(void)
 * @brief Check switch feature is support or not.
 *
 * This function is called to inquiry switch feature is support or not.
 * @retval 1 Switch support.
 * @retval 0 Switch not support.
 */
int HAL_MB_Is_Switch_Support(void);

/**
 * @fn int HAL_MB_Get_Switch_MAC(OUT char* mac_addr, IN unsigned int buf_len)
 * @brief Retrieve the Switch MAC address.
 *
 * This function will retrieve the Swtich MAC address .
 * @param[out] mac_addr The retrived Switch MAC address will be stored here.
 * @param[in] buf_len The length of output string buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Switch_MAC(OUT char* mac_addr, IN unsigned int buf_len);

/**
 * @fn int HAL_MB_Set_Switch_MAC(IN char* mac_addr)
 * @brief Set up the Switch MAC address.
 *
 * This function will set up the Switch MAC address.
 * @param[in] mac_addr The MAC address that will be written.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_Switch_MAC(IN char* mac_addr);

/**
 * @fn int HAL_MB_Get_Button_Count(void)
 * @brief Retrieve the number of buttons for this NAS motherboard.
 *
 * This function will retrieve the number of buttons and get button name.
 * @retval >= 0 Success, return the count of the available buttons.
 */
int HAL_MB_Get_Button_Count(void);

/**
 * @fn int HAL_MB_Get_Button_Name(IN int index, OUT char* name, IN int name_buf_size)
 * @brief Retrieve the name of buttons for this NAS motherboard.
 *
 * This function will retrieve the button name by index.
 * @param[in] index The index of button, 0 based.
 * @param[out] name The pointer of the name buffer.
 * @param[in] name_buf_size The buffer size of name buffer.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Button_Name(IN int index, OUT char* name, IN int name_buf_size);

/**
 * @fn int HAL_MB_Is_FAN_Region_Support(OUT int* fan_region)
 * @brief Check if the FAN control region is supported
 *
 * This function is called to inquiry if the FAN control region is supported or not.
 * @param[out] fan_region Total fan regions.
 * @retval 1 Fan region support.
 * @retval 0 Fan region not support.
 */
int HAL_MB_Is_FAN_Region_Support(OUT int* fan_region);

/**
 * @fn int HAL_MB_Is_Temp_Region_Support(OUT int* temp_region)
 * @brief Check if the multiple system Temp Region is supported
 *
 * This function is called to inquiry if the multiple system Temp Region is supported or not.
 * @param[out] temp_region Total temp. regions.
 * @retval 1 Temp. region support.
 * @retval 0 Temp. region not support.
 */
int HAL_MB_Is_Temp_Region_Support(OUT int* temp_region);

/**
 * @fn int HAL_MB_Is_TFAN_Supported(OUT int* fan_region)
 * @brief Check if the FAN control region is supported
 *
 * This function is called to inquiry if the FAN control region is supported  is support or not.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 1 TFan is supportted.
 * @retval 0 TFan is not supported.
 */
int HAL_MB_Is_TFAN_Supported(IN FAN_REGION region);

/**
 * @fn int HAL_MB_Is_Scrollbar_Supported(OUT int* fan_region)
 * @brief Check if the scrollbar is supported in the fixed fan mode of the specified fan region
 *
 * This function is called to inquiry if the scrollbar is supported in the fixed fan mode of the specified fan region.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 1 Scrollbar is supportted.
 * @retval 0 Scrollbar is not supported.
 */
int HAL_MB_Is_Scrollbar_Supported(IN FAN_REGION region);

/**
 * @fn int HAL_MB_Is_TFAN_Optional_Supported(OUT int* fan_region)
 * @brief Check if the fan algorithm is TFan, and supports different mode for the specified fan region
 *
 * This function is called to inquiry if the fan algorithm is TFan, and supports different mode for the specified fan region.
 * @param[in] region The specified fan region, which stands for SysFan or CPUFan.
 * @retval 1 TFan mode is supportted.
 * @retval 0 TFan mode is not supported.
 */
int HAL_MB_Is_TFAN_Optional_Supported(IN FAN_REGION region);

/**
 * @fn int HAL_MB_Is_ES_Boot_Support(void)
 * @brief Check ES boot feature is support or not.
 *
 * This function is called to inquiry ES boot feature is support or not.
 * @retval 1 ES boot support.
 * @retval 0 ES boot not support.
 */
int HAL_MB_Is_ES_Boot_Support(void);

/**
 * @fn int HAL_MB_Prepare_ES_Boot(void)
 * @brief Prepare to boot ES firmware.
 *
 * This function is called to prepare to boot ES firmware.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Prepare_ES_Boot(void);

/**
 * @fn int HAL_MB_Is_Buzzer_BIOS_Setting_Support(void)
 * @brief Check if setting buzzer during booting feature is support or not.
 *
 * This function is called to inquiry buzzer setting during booting feature
 * is support or not.
 * @retval 1 setting support.
 * @retval 0 setting not support.
 */
int HAL_MB_Is_Buzzer_BIOS_Setting_Support(void);

/**
 * @fn int HAL_MB_Get_Buzzer_Bios_Setting(OUT int *enabled)
 * @brief get buzzer setting in bios.
 *
 * This function is called to get whether buzzer is enabled during booting
 * @param[out] 1 enabled, 0 disabled
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Get_Buzzer_Bios_Setting(OUT int *enabled);

/**
 * @fn int HAL_MB_Set_Buzzer_Bios_Setting(IN int enable)
 * @brief get buzzer setting in bios.
 *
 * This function is called to set buzzer is enabled or not during booting
 * @param[in] 1 enable, 0 disable
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Set_Buzzer_Bios_Setting(IN int enable);

/**
 * @fn int HAL_MB_Sound_Alert(IN BUZZER_MODE mode)
 * @brief Set the speaker or buzzer with the specified voice_id.
 *
 * @param[in] voice_id ID of the voice.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Sound_Alert(IN VOICE_ID voice_id);

/**
 * @fn int HAL_MB_Fixup_Platform(void)
 * @brief Fixup platform initialization if needed.
 *
 * This function is called to fixup platform initialization if needed.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_MB_Fixup_Platform(void);

/**
 * @fn int HAL_MB_Is_USB_COPY_BUTTON_Support(void)
 * @brief Check usb copy button feature is support or not.
 *
 * This function is called to inquiry usb copy button feature is support or not.
 * @retval 1 usb copy button support.
 * @retval 0 usb copy button not support.
 */
int HAL_MB_Is_USB_COPY_BUTTON_Support(void);

/**
 * @fn int HAL_MB_Is_FAN_UNIT_Support(void)
 * @brief Check fan unit feature is support or not.
 *
 * This function is called to inquiry fan unit feature is support or not.
 * @retval 1 fan unit support.
 * @retval 0 fan unit not support.
 */
int HAL_MB_Is_FAN_UNIT_Support(void);

/**
 * @fn int HAL_MB_Is_FC_Support(void)
 * @brief Check FC HBA is support or not
 *
 * This function is called to inquiry FC HBA is support or not.
 * @retval 1 FC HBA support.
 * @retval 0 FC HBA not support.
 */
int HAL_MB_Is_FC_Support(void);


/*@}*/  /* end of motherboard module */

/**
 * @defgroup HAL_MONITOR Monitor function
 */
/*@{*/  /* begin of monitor module */

/**
 * @enum      HAL_MONITOR_MODE
 * @brief     HAL Monitor execution mode
 */
typedef enum _HAL_MONITOR_MODE
{
    HAL_MM_UNKNOWN = -1,    /*!< Unknown execution mode. */
    HAL_MM_NORMAL = 0,      /*!< Normal service mode. */
    HAL_MM_TEST = 1,        /*!< Test service mode. */
    HAL_MM_STOP = 2,        /*!< Stop mode. */
} HAL_MONITOR_MODE;

/**
 * @fn int HAL_Monitor_Change_Mode(IN HAL_MONITOR_MODE mode)
 * @brief Change the daemon execution mode.
 *
 * This function is called by hwtest to ask HAL daemon to enter the specified service mode.
 * @param[in] mode specific service mode.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Monitor_Change_Mode(IN HAL_MONITOR_MODE mode);

/**
 * @fn int HAL_Monitor_Set_Mode(IN HAL_MONITOR_MODE mode)
 * @brief Write down the service mode into the internal configuration file.
 *
 * This function is called by HAL daemon to record the current service mode.
 * @param[in] mode specific service mode.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Monitor_Set_Mode(IN HAL_MONITOR_MODE mode);

/**
+ * @fn int HAL_MB_Set_BREATH_LED(IN int enable)
+ * @brief Enable/Disable Breath led.
+ *
+ * This function will enable/disable breath led.
+ * @param[in] enable enable/disable.
+ * @retval 0 Success.
+ */
int HAL_MB_Set_BREATH_LED(IN int enable);

/**
 * @fn HAL_MONITOR_MODE HAL_Monitor_Get_Mode(void)
 * @brief Retrieve the current service mode.
 *
 * This function is called by hal monitor/hwtest to retrieve the current service mode of the hal daemon.
 * @retval The current service mode of the hal demon.
 */
HAL_MONITOR_MODE HAL_Monitor_Get_Mode(void);

/*@}*/  /* end of monitor module */

/**
 * @defgroup HAL_PD_SMART HDD SMART Info function
 */
/*@{*/  /* begin of HDD SMART Info module */

/**
 * @enum        PD_HEALTH_STATUS
 * @brief       Physical disk health status
 */
typedef enum _PD_HEALTH_STATUS
{
    PD_HEALTH_OK = 0,                /*!< OK status. */
    PD_HEALTH_NORMAL,                /*!< Nornal status. */
    PD_HEALTH_ABNORMAL,              /*!< Abnormal status. */
} PD_HEALTH_STATUS;

/**
 * @enum        SMART_ATTRIBUTE_STATUS
 * @brief       Physical disk smart attribute status
 */
typedef enum _SMART_ATTRIBUTE_STATUS
{
    SMART_ATTRIBUT_OK = 0,                /*!< OK status. */
    SMART_ATTRIBUT_NORMAL,                /*!< Nornal status. */
    SMART_ATTRIBUT_ABNORMAL,              /*!< Abnormal status. */
} SMART_ATTRIBUTE_STATUS;

/**
 * @enum    SMART_WARNING_POLICY
 * @brief   SMART warning select policy.
 */
typedef enum _SMART_WARNING_POLICY
{
    SMART_WARNING_DEFAULT = 0, /*!< only check S.M.A.R.T. attribute #197 and #198 raw value. */
    SMART_WARNING_STRICT,      /*!< also check S.M.A.R.T. attribute #5, #187 and #188 raw value. */
    SMART_WARNING_MANUALLY,    /*!< check S.M.A.R.T. specific attribute raw value. */
} SMART_WARNING_POLICY;

/**
 * @enum    PD_NOTIFIY_EVENT
 * @brief   Enumeration for the warning/ error events which might happen on the physical disk 
 */
typedef enum _PD_NOTIFIY_EVENT
{
    PD_NOTIFIY_NO_EVENT = 0, /*!< only check S.M.A.R.T. attribute #197 and #198 raw value. */
    PD_NOTIFIY_EVENT_WARNING_START,
    PD_NOTIFIY_EVENT_WARNING_ABNORMAL_SECTORS,
    PD_NOTIFIY_EVENT_WARNING_OVER_HEAT,
    PD_NOTIFIY_EVENT_WARNING_SMART_ATTR_WARNING,
    PD_NOTIFIY_EVENT_WARNING_NO_SMART_ATTR,
    PD_NOTIFIY_EVENT_WARNING_SELF_TEST_ABNORMAL,
    PD_NOTIFIY_EVENT_ERROR_START,
    PD_NOTIFIY_EVENT_ERROR_IO_ERROR,
    PD_NOTIFIY_EVENT_ERROR_OVER_HEAT,
    PD_NOTIFIY_EVENT_ERROR_SMART_ATTR_ABNORMAL,
    PD_NOTIFIY_EVENT_ERROR_HEALTH_ABNORMAL,				// For SAS/ NVME disks
    PD_NOTIFIY_EVENT_ERROR_FAILED_TO_RETRIEVE_SMART,
    PD_NOTIFIY_EVENT_ERROR_UNCOVERABLE_READ_ERRORS,
    PD_NOTIFIY_EVENT_ERROR_MEDIUM_ERROR,
    PD_NOTIFIY_EVENT_NUM,
} PD_NOTIFIY_EVENT;

/**
 * @struct      PD_SMART_ATTR
 * @brief       HDD S.M.A.R.T Information, please refer section
 *              6.54 of ATA specification volume one.
 */
typedef struct _PD_SMART_ATTR {
    unsigned int attr_id;                       /*!< */
    char attr_name[MAX_SMART_ATTR_NAME_LEN];    /*!< */
    unsigned char attr_value;                   /*!< */
    unsigned char attr_worst;                   /*!< */
    unsigned char attr_threshold;               /*!< */
    unsigned long long attr_raw;                /*!< */
    unsigned short attr_flags;                  /*!< */
    unsigned short attr_status;                 /*!< */
} PD_SMART_ATTR;

/**
 * @struct      PD_SMART_INFO
 * @brief       HDD S.M.A.R.T Information, please refer section
 *              6.54 of ATA specification volume one.
 */
typedef struct _PD_SMART_INFO {
    PD_TYPE pd_type;
    PD_HEALTH_STATUS summary;
    PD_ATA_SELFTEST_STATUS test_status;
    int test_percentage;
    int temp;
    /*io_status:
             if IO_ERROR ==>PD_HEALTH_ABNORMAL,
          else  ==>PD_HEALTH_OK
       */
    PD_HEALTH_STATUS io_status;
    /*smart_status:
          if one attribute abnormal || self_test abnormal ==>PD_HEALTH_ABNORMAL,
          else  ==>PD_HEALTH_OK
       */
    PD_HEALTH_STATUS smart_status;
    /*temp_status:
          if (pd_temp <= SMART_NORMAL_TEMP_THRESH && pd_temp <= pd->temp_threshold) ==>PD_HEALTH_OK
        elif (pd_temp >= SMART_ABNORMAL_TEMP_THRESH) ==>PD_HEALTH_ABNORMAL
        else ==> PD_HEALTH_NORMAL
       */
    PD_HEALTH_STATUS temp_status;
    /*health_status:
          SATA use SMART_RETURN_STATUS, SAS use REQUEST_SENSE
          Format is ASC | ASCQ
       */
    int health_status;
    int short_test_minute;
    int extended_test_minute;
    int attr_count;
    PD_NOTIFIY_EVENT latest_pd_notify_event;
    PD_SMART_ATTR attr_ary[0];
} PD_SMART_INFO;

/**
 * @struct      SMART_SUMMARY
 * @brief       move from qsmart.h
 */
typedef struct smart_summary {
	int 		hd_no;
	int 		sum_health;
	char 		vendor[64];
	float 		cap;	// GB
	int 		health;
	int     	temperature;
	time_t		last_test_time;
	int 		last_test_type;
	int 		last_test_sts;
	int 		temp_type;
}SMART_SUMMARY;

/**
 * @struct      SMART_CONF
 * @brief       move from qsmart.h
 */
typedef struct smart_conf {
	int hd_no;
	int en_warn_temp;
	int temp_thresh;
    int temp_thresh_f;
	int temp_type;
	int en_short;
	unsigned long long short_sch;
	int en_ext;
	unsigned long long ext_sch;
}SMART_CONF;

/**
 * @struct    SMART_WARNING_ID
 * @brief     SMART warning attribute.
 */
typedef struct _SMART_WARNING_ID
{
    int enable; /*!< enable or not (1 -> enable, 0 -> disable). */
    int value;  /*!< warning threshold. */
} SMART_WARNING_ID;

/**
 * @struct    SMART_WARNING_CONF
 * @brief     SMART warning configure.
 */
typedef struct _SMART_WARNING_CONF
{
    int policy;              /*!< select policy. */
    SMART_WARNING_ID ID_5;   /*!< attribute #5. */
    SMART_WARNING_ID ID_187; /*!< attribute #187. */
    SMART_WARNING_ID ID_188; /*!< attribute #188. */
    SMART_WARNING_ID ID_197; /*!< attribute #197. */
    SMART_WARNING_ID ID_198; /*!< attribute #198. */
} SMART_WARNING_CONF;

/**
 * @fn unsigned char PD_SMART_Get_Temp_Thres(int enc_id, int port_id)
 * @brief Retrieve the threshold value of specified physical harddisk.
 *
 * This function will be invoked to retrieve the threshold value of specified physical harddisk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval unsigned value of temp threshold.
 */
unsigned char PD_SMART_Get_Temp_Thres(int enc_id, int port_id);

/**
 * @fn unsigned char PD_SMART_Get_Temp_Thres_F(int enc_id, int port_id)
 * @brief Retrieve the threshold value ('F) of specified physical harddisk.
 *
 * This function will be invoked to retrieve the threshold value ('F) of specified physical harddisk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval unsigned value of temp threshold.
 */
unsigned char PD_SMART_Get_Temp_Thres_F(int enc_id, int port_id);

/**
 * @fn unsigned long long PD_SMART_Get_Test_Schedule(int enc_id, int port_id, int ttype, int *day_type, int *day, int *hour, int *min, int *interval)
 * @brief Retrieve the schedule information from config file.
 *
 * This function will be invoked to retrieve the schedule information from config file.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] ttype testing type.
 * @param[out] day_type day type.
 * @param[out] day day value.
 * @param[out] hour hour value.
 * @param[out] min min value.
 * @param[out] interval interval value.
 * @retval a unsigned long long integer value of schedule.
 */
unsigned long long PD_SMART_Get_Test_Schedule(IN int enc_id, IN int port_id, IN int ttype, OUT int *day_type, OUT int *day, OUT int *hour, OUT int *min, OUT int *interval);

/**
 * @fn int PD_SMART_Get_Conf(int enc_id, int port_id, struct smart_conf *sc)
 * @brief Retrieve the smart_conf for legacy compatible.
 *
 * This function will be invoked to retrieve the smart_conf for legacy compatible.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] sc a pointer to struct smart_conf.
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_SMART_Get_Conf(int enc_id, int port_id, struct smart_conf *sc);

/**
 * @fn int PD_SMART_Is_Temp_Thresh_Enabled(int enc_id, int port_id)
 * @brief Retrieve the status of hdd temperature alert.
 *
 * This function will be invoked to retrieve status of hdd temperature alert.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval 1 the threshold is enable.
 * @retval 0 the threshold is disable.
 */
int PD_SMART_Is_Temp_Thresh_Enabled(int enc_id, int port_id);

/**
 * @fn int PD_SMART_Is_Schedule_Enabled(int enc_id, int port_id, int type)
 * @brief Retrieve the status of test schedule by different testing type.
 *
 * This function will be invoked to retrieve the status of test schedule by different testing type.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[in] type the test type of schedule (defined in qsmart.h).
 * @retval 1 the schedule is enable.
 * @retval 0 the schedule is disable.
 */
int PD_SMART_Is_Schedule_Enabled(int enc_id, int port_id, int type);

/**
 * @fn int PD_SMART_Get_Last_Test_Type(int enc_id, int port_id)
 * @brief Retrieve the last test type from config file.
 *
 * This function will be invoked to retrieve the last test type from config file.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval integer, define in qsmart.h.
 */
int PD_SMART_Get_Last_Test_Type(int enc_id, int port_id);

/**
 * @fn int PD_SMART_Get_Now_Test_Type(int enc_id, int port_id)
 * @brief Retrieve the current test type from config file.
 *
 * This function will be invoked to retrieve the current test type from config file.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @retval integer, define in qsmart.h.
 */
int PD_SMART_Get_Now_Test_Type(int enc_id, int port_id);

/**
 * @fn int PD_SMART_Get_Summary(IN int enc_id, IN int port_id, OUT PD_SMART_SUMMARY* summaryP)
 * @brief Retrieve the S.M.A.R.T summary for the specified physical disk.
 *
 * This function will be invoked to retrieve the S.M.A.R.T summary according to the underlying
 * transport.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] summaryP The S.M.A.R.T summary value.
 * @retval 0 Success, the S.M.A.R.T summary of the specified physical disk is retrieved completely.
 * @retval -1 Generic failure.
 * @retval -2 Open smart file fail, not ready.
 */
int PD_SMART_Get_Summary(IN int enc_id, IN int port_id, OUT struct smart_summary *sm);

/**
 * @fn unsigned char PD_SMART_Get_Temperature(PD_SMART_INFO *smart_infoP)
 * @brief Retrieve the S.M.A.R.T temperature for the specified physical disk.
 *
 * This function will be invoked to retrieve temperature from a PD_SMART_INFO struct.
 * @param[in] smart_infoP a pointer to a PD_SMART_INFO struct.
 * @retval >0 Success, the temperature value in Celsius.
 * @retval 0 Generic failure.
 */
unsigned char PD_SMART_Get_Temperature(PD_SMART_INFO *smart_infoP);

/**
 * @fn int PD_SMART_Get_ATA_Info(IN int enc_id, IN int port_id, OUT PD_ATA_SMART_RETURN_STATUS* smart_return_status,
 *                               OUT PD_SMART_ATTR smart_attr_ary[], IN int attr_ary_count,
 *                               OUT PD_ATA_SELFTEST_STATUS* test_statusP, OUT int* test_percentageP,
 *                               OUT int* test_short_estimateP, OUT int* test_extend_extimateP)
 *
 * @brief Enumerate available ATA S.M.A.R.T attributes and self test status for specified ATA physical disk.
 *
 * This function is called by nas utility to obtain all available ATA S.M.A.R.T attributes
 * and self test status of the physical disk.
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[in] port_id Physical disk port identifer (1 based).
 * @param[out] smart_return_status smart return status.
 * @param[out] smart_attr_ary An S.M.A.R.T attribute array to host the available ATA S.M.A.R.T attributes.
 * @param[in] attr_ary_count The entry count of the attribute array.
 * @param[out] test_statusP The self test status if it is proceeding.
 * @param[out] test_percentageP The percent of the self-test routine remaining until completion in ten percent increments.
 * @param[out] test_short_estimateP The short self test recommended polling time(in minutes)
 * @param[out] test_extend_extimateP The extended self test recommended polling time(in minutes)
 * @retval >=0 Success, return the count of the available ATA S.M.A.R.T attributes, if the count is larger than
 *         attr_ary_count, the calling utility should allocate more space for attribute array to
 *         obtain all available attributes.
 * @retval Generic failure.
 */
int PD_SMART_Get_ATA_Info(IN int enc_id, IN int port_id, OUT PD_ATA_SMART_RETURN_STATUS *smart_return_status,
                          OUT PD_SMART_ATTR smart_attr_ary[], IN int attr_ary_count,
                          OUT PD_ATA_SELFTEST_STATUS* test_statusP, OUT int* test_percentageP,
                          OUT int* test_short_estimateP, OUT int* test_extend_extimateP);

/**
 * @fn int PD_SMART_Read_Info(IN int enc_id, IN int port_id, OUT PD_SMART_INFO **smart_infoPP)
 * @brief Write the S.M.A.R.T information to file.
 *
 * This function is called by nas utility to write all available S.M.A.R.T attributes to file.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[out] smart_infoPP SMART Information, the memory space shoud be free after using on user own.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 * @retval -2 Open smart file fail, not ready.
 */
/****************************** IMPORTANT ********************************
    Please remember free the memory space of smart_infoPP before caller terminate!!
*************************************************************************/
int PD_SMART_Read_Info(IN int enc_id, IN int port_id, OUT PD_SMART_INFO **smart_infoPP);

/**
 * @fn int PD_SMART_Get_Current_Notify_Event(IN int enc_id, IN int port_id, OUT PD_NOTIFIY_EVENT *pd_notify_event)
 * @brief Get the latest notify event of the specified PD.
 *
 * This function is called to get notify event of the specified port.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[out] the latest pd notify event.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 * @retval -2 Open smart file fail, not ready.
*/
int PD_SMART_Get_Current_Notify_Event(IN int enc_id, IN int port_id, OUT PD_NOTIFIY_EVENT *pd_notify_event);

/**
 * @fn int PD_SMART_Set_Current_Notify_Event(IN int enc_id, IN int port_id, IN PD_NOTIFIY_EVENT pd_notify_event)
 * @brief Set the latest notify event to the specified PD.
 *
 * This function is called to set notify event of the specified port.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[in] the latest pd notify event.
 * @retval 0 Success.
 * @retval < 0 Generic failure.
*/
int PD_SMART_Set_Current_Notify_Event(IN int enc_id, IN int port_id, IN PD_NOTIFIY_EVENT pd_notify_event);

/**
 * @fn int PD_SMART_Get_Power_On_Hours(IN int enc_id, IN int port_id, OUT unsigned int *hours)
 * @brief Get the number of power-on hours.
 *
 * This function is called to get the number of power-on hours.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[out] hours accumulated power on hours.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 * @retval -2 Open smart file fail, not ready.
*/
int PD_SMART_Get_Power_On_Hours(IN int enc_id, IN int port_id, OUT unsigned int *hours);

/**
 * @fn int PD_SMART_Clear_Info(IN int enc_id, IN int port_id)
 * @brief Clear the S.M.A.R.T information file.
 *
 * This function is called by nas utility to remove file about S.M.A.R.T attributes.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_SMART_Clear_Info(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SMART_Write_Info(IN int enc_id, IN int port_id, IN PD_SMART_INFO *smart_infoP);
 * @brief Read the S.M.A.R.T information from file.
 *
 * This function is called by nas utility to write all available S.M.A.R.T attributes to file.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.
 * @param[in] smart_infoP SMART Information

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_SMART_Write_Info(IN int enc_id, IN int port_id, IN PD_SMART_INFO *smart_infoP);

/**
 * @fn int PD_SMART_Is_Error(IN int enc_id, IN int port_id);
 * @brief Check if the specified HDD has IO error record.
 *
 * This function is called to check if the specified HDD has IO error record.
 * @param[in] enc_id Enclosure identifier.
 * @param[in] port_id Physical disk port identifer.

 * @retval 1 TRUE, the specified HDD has IO error record.
 * @retval 0 FALSE, the specified HDD doesn't have IO error record.
 */
int PD_SMART_Is_Error(IN int enc_id, IN int port_id);

/**
 * @fn int PD_SMART_WARNING_SET_CONF(IN SMART_WARNING_CONF *warningP)
 * @brief Set S.M.A.R.T. warning policy global setting.
 *
 * This function is called by set S.M.A.R.T. warning policy.
 * @param[in] warningP Data struct for configure content.
 * @retval 0 Success, the specified config is set successfully.
 * @retval -1 Generic failure, fail to set the specified config.
 */
int PD_SMART_WARNING_SET_CONF(IN SMART_WARNING_CONF *warningP);

/**
 * @fn int PD_SMART_WARNING_GET_CONF(OUT SMART_WARNING_CONF *warningP)
 * @brief Get S.M.A.R.T. warning policy global setting.
 *
 * This function is called by get S.M.A.R.T. warning policy.
 * @param[out] warningP Data struct for configure content.
 * @retval 0 Success, the specified config is get successfully.
 * @retval -1 Generic failure, fail to get the specified config.
 */
int PD_SMART_WARNING_GET_CONF(OUT SMART_WARNING_CONF *warningP);

/*@}*/  /* end of HDD SMART Info module */

/**
 * @defgroup VPD VPD function
 */

/**
 * @ingroup VPD VPD function
 */
/*@{*/  /* begin of VPD module */

#define VPD_ROM_SIZE  256

/**
 * @typedef U8
 * @brief   The basic access unit of VPD structure.
 */
typedef unsigned char U8;

/**
 * @enum        VPD_Data_Type
 * @brief       VPD Data Type.
 */
typedef enum _VPD_Data_Type
{
    VPD_DATA_TYPE_BINARY = 0,   /*!< Binary Data (ex: integer) */
    VPD_DATA_TYPE_BCDPLUS,      /*!< BCD Plus Data */
    VPD_DATA_TYPE_6BITASCII,    /*!< 6-bits ASCII Data */
    VPD_DATA_TYPE_8BITASCII,    /*!< 8-bits ASCII Data (ex: common string) */
} VPD_Data_Type;

/**
 * @struct      VPD_Header
 * @brief       VPD Common Header structure.
 */
typedef struct _VPD_Header
{
    U8  vpd_version;        /*!< Common Header Format Version. */
    U8  internal_offset;    /*!< Internal Use Area Starting Offset (in multiples of 8 bytes). */
    U8  chassis_offset;     /*!< Chassis Info Area Starting Offset (in multiples of 8 bytes). */
    U8  board_offset;       /*!< Board Area Starting Offset (in multiples of 8 bytes). */
    U8  product_offset;     /*!< Product Info Area Starting Offset (in multiples of 8 bytes). */
    U8  multi_offset;       /*!< MultiRecord Area Starting Offset (in multiples of 8 bytes). */
    U8  padding[1];         /*!< PAD, write as 00h */
    U8  cksum;              /*!< Common Header Checksum (zero checksum). */
} VPD_Header;

/**
 * @struct      VPD_MB_Board
 * @brief       VPD Main Board - Board Area structure.
 */
typedef struct _VPD_MB_Board
{
    // VPD Area
    U8  vpd_version;        /*!< Board Area Format Version. */
    U8  area_length;        /*!< Board Area Length (in multiples of 8 bytes). */
    U8  language;           /*!< Language Code. */
    U8  mfg_date[3];        /*!< Mfg. Date / Time. */
    // Data Area
    U8  manufacturer_t;     /*!< Board Manufacturer type/length byte. */
    U8  manufacturer_d[16]; /*!< Board Manufacturer bytes. */
    U8  product_name_t;     /*!< Board Product Name type/length byte. */
    U8  product_name_d[16]; /*!< Board Product Name bytes. */
    U8  serial_number_t;    /*!< Board Serial Number type/length byte. */
    U8  serial_number_d[16];/*!< Board Serial Number bytes. */
    U8  part_number_t;      /*!< Board Part Number type/length byte. */
    U8  part_number_d[32];  /*!< Board Part Number bytes. */
    U8  fru_file_id_t;      /*!< FRU File ID type/length byte. */
    U8  fru_file_id_d[0];   /*!< FRU File ID bytes (NO USE). */
    U8  sas_address_t;      /*!< SAS Address type/length byte. */
    U8  sas_address_d[16];  /*!< SAS Address bytes. */
    // VPD Area
    U8  end_flag;           /*!< C1h (type/length byte encoded to indicate no more info fields). */
    U8  padding[2];         /*!< PAD, write as 00h. */
    U8  cksum;              /*!< Board Area Checksum (zero checksum). */
} VPD_MB_Board;

/**
 * @struct      VPD_MB_Product
 * @brief       VPD Main Board - Product Area structure.
 */
typedef struct _VPD_MB_Product
{
    // VPD Area
    U8  vpd_version;            /*!< Product Area Format Version. */
    U8  area_length;            /*!< Product Area Length (in multiples of 8 bytes). */
    U8  language;               /*!< Language Code. */
    // Data Area
    U8  manufacturer_t;         /*!< Manufacturer Name type/length byte. */
    U8  manufacturer_d[16];     /*!< Manufacturer Name bytes. */
    U8  product_name_t;         /*!< Product Name type/length byte. */
    U8  product_name_d[32];     /*!< Product Name bytes. */
    U8  part_model_number_t;    /*!< Product Part/Model Number type/length byte. */
    U8  part_model_number_d[16];/*!< Product Part/Model Number bytes. */
    U8  product_version_t;      /*!< Product Version type/length byte. */
    U8  product_version_d[3];   /*!< Product Version bytes. */
    U8  serial_number_t;        /*!< Product Serial Number type/length byte. */
    U8  serial_number_d[16];    /*!< Product Serial Number bytes. */
    U8  asset_tag_t;            /*!< Asset Tag type/length byte. */
    U8  asset_tag_d[0];         /*!< Asset Tag (NO USE). */
    U8  fru_file_id_t;          /*!< FRU File ID type/length byte. */
    U8  fru_file_id_d[0];       /*!< FRU File ID bytes (NO USE). */
    U8  product_nickname_t;     /*!< Product Nickname type/length byte. */
    U8  product_nickname_d[16]; /*!< Product Nickname bytes. */
    // VPD Area
    U8  end_flag;               /*!< C1h (type/length byte encoded to indicate no more info fields). */
    U8  padding[1];             /*!< PAD, write as 00h. */
    U8  cksum;                  /*!< Product Info Area Checksum (zero checksum). */
} VPD_MB_Product;

/**
 * @struct      VPD_MB_Rom
 * @brief       VPD Main Board ROM structure.
 */
typedef struct _VPD_MB_Rom
{
    VPD_Header      header;     /*!< VPD Common Header structure. */
    VPD_MB_Board    board;      /*!< VPD Main Board - Board Area structure. */
    VPD_MB_Product  product;    /*!< VPD Main Board - Product Area structure. */
} VPD_MB_Rom;

/**
 * @struct      VPD_BP_Chassis
 * @brief       VPD Backplane - Chassis Area structure.
 */
typedef struct _VPD_BP_Chassis
{
    // VPD Area
    U8  vpd_version;            /*!< Chassis Info Area Format Version. */
    U8  area_length;            /*!< Chassis Info Area Length (in multiples of 8 bytes). */
    U8  type;                   /*!< Chassis Type. (ex: Tower:0x07, Rackmount:0x11, REXP:0x0c)*/
    // Data Area
    U8  part_number_t;          /*!< Chassis Part Number type/length. */
    U8  part_number_d[16];      /*!< Chassis Part Number bytes. */
    U8  serial_number_t;        /*!< Chassis Serial Number type/length. */
    U8  serial_number_d[16];    /*!< Chassis Serial Number bytes. (QNAP Serial Number) */
    // VPD Area
    U8  end_flag;               /*!< C1h (type/length byte encoded to indicate no more info fields). */
    U8  padding[1];             /*!< PAD, write as 00h. */
    U8  cksum;                  /*!< Chassis Info Checksum (zero checksum). */
} VPD_BP_Chassis;

/**
 * @struct      VPD_BP_Board
 * @brief       VPD Backplane - Board Area structure.
 */
typedef struct _VPD_BP_Board
{
    // VPD Area
    U8  vpd_version;        /*!< Board Area Format Version. */
    U8  area_length;        /*!< Board Area Length (in multiples of 8 bytes). */
    U8  language;           /*!< Language Code. */
    U8  mfg_date[3];        /*!< Mfg. Date / Time. */
    // Data Area
    U8  manufacturer_t;     /*!< Board Manufacturer type/length byte. */
    U8  manufacturer_d[16]; /*!< Board Manufacturer bytes. */
    U8  product_name_t;     /*!< Board Product Name type/length byte. */
    U8  product_name_d[16]; /*!< Board Product Name bytes. */
    U8  serial_number_t;    /*!< Board Serial Number type/length byte. */
    U8  serial_number_d[16];/*!< Board Serial Number bytes. */
    U8  part_number_t;      /*!< Board Part Number type/length byte. */
    U8  part_number_d[32];  /*!< Board Part Number bytes. */
    U8  fru_file_id_t;      /*!< FRU File ID type/length byte. */
    U8  fru_file_id_d[0];   /*!< FRU File ID bytes (NO USE). */
    // VPD Area
    U8  end_flag;           /*!< C1h (type/length byte encoded to indicate no more info fields). */
    U8  padding[3];         /*!< PAD, write as 00h. */
    U8  cksum;              /*!< Board Area Checksum (zero checksum). */
} VPD_BP_Board;

/**
 * @struct      VPD_BP_Product
 * @brief       VPD Backplane - Product Area structure.
 */
typedef struct _VPD_BP_Product
{
    // VPD Area
    U8  vpd_version;            /*!< Product Area Format Version. */
    U8  area_length;            /*!< Product Area Length (in multiples of 8 bytes). */
    U8  language;               /*!< Language Code. */
    // Data Area
    U8  manufacturer_t;         /*!< Manufacturer Name type/length byte. */
    U8  manufacturer_d[16];     /*!< Manufacturer Name bytes. */
    U8  product_name_t;         /*!< Product Name type/length byte. */
    U8  product_name_d[16];     /*!< Product Name bytes. */
    U8  part_model_number_t;    /*!< Product Part/Model Number type/length byte. */
    U8  part_model_number_d[16];/*!< Product Part/Model Number bytes. */
    U8  product_version_t;      /*!< Product Version type/length byte. */
    U8  product_version_d[3];   /*!< Product Version bytes. */
    U8  serial_number_t;        /*!< Product Serial Number type/length byte. */
    U8  serial_number_d[16];    /*!< Product Serial Number bytes. */
    U8  asset_tag_t;            /*!< Asset Tag type/length byte. */
    U8  asset_tag_d[0];         /*!< Asset Tag (NO USE). */
    U8  fru_file_id_t;          /*!< FRU File ID type/length byte. */
    U8  fru_file_id_d[0];       /*!< FRU File ID bytes (NO USE). */
    // VPD Area
    U8  end_flag;               /*!< C1h (type/length byte encoded to indicate no more info fields). */
    U8  padding[1];             /*!< PAD, write as 00h. */
    U8  cksum;                  /*!< Product Info Area Checksum (zero checksum). */
} VPD_BP_Product;

/**
 * @struct      VPD_BP_Rom
 * @brief       VPD Backplane ROM structure.
 */
typedef struct _VPD_BP_Rom
{
    VPD_Header      header;     /*!< VPD Common Header structure. */
    VPD_BP_Chassis  chassis;    /*!< VPD Backplane - Chassis Area structure. */
    VPD_BP_Board    board;      /*!< VPD Backplane - Board Area structure. */
    VPD_BP_Product  product;    /*!< VPD Backplane - Product Area structure. */
} VPD_BP_Rom;

/**
 * @fn int VPD_MfgDate_To_Binary(IN char *time_str, IN int time_str_size, OUT unsigned char *output, IN int size)
 * @brief Transform date time structure to the binary value of minutes diff by 2013/1/1.
 *
 * In the VPD structure, it has 3 bytes space to recored the manufacure date, we can use this function
 * to transform the date time structure to 3 bytes value.
 * @param[in] time_str Date time string.
 * @param[in] time_str_size Date time string buffer size.
 * @param[out] output Output buffer.
 * @param[in] size Output buffer size.

 * @retval 0 Success, transform successfully.
 * @retval -1 Generic failure.
 */
int VPD_MfgDate_To_Binary(IN char *time_str, IN int time_str_size, OUT unsigned char *output, IN int size);

/**
 * @fn int VPD_Binary_To_Mfgdate(OUT char *time_str, IN int time_str_size, IN unsigned char *input, IN int size)
 * @brief Transform binary value in vpd to date time string.
 *
 * This function is called to transform binary value in vpd to date time string.
 * @param[out] time_str Date time string.
 * @param[in] time_str_size Date time string buffer size.
 * @param[in] input Input buffer.
 * @param[in] size Input buffer size.

 * @retval 0 Success, transform successfully.
 * @retval -1 Generic failure.
 */
int VPD_Binary_To_Mfgdate(OUT char *time_str, IN int time_str_size, IN unsigned char *input, IN int size);

/**
 * @fn int VPD_MB_Data_Validator(VPD_MB_Rom *rom)
 * @brief Validate the Main Board VPD ROM.
 *
 * This function will check all entries of entire rom of main board VPD, and calculate checksum.
 * @param[in] rom Double pointer of Date time structure.

 * @retval 0 Success, the rom is validated.
 * @retval !0 Generic failure, the rom is not validated, or the data is modified.
 */
int VPD_MB_Data_Validator(INOUT VPD_MB_Rom *rom);

/**
 * @fn int VPD_BP_Data_Validator(INOUT VPD_BP_Rom *rom)
 * @brief Validate the Main Board VPD ROM.
 *
 * This function will check all entries of entire rom of main board VPD, and calculate checksum.
 * @param[in] rom Double pointer of Date time structure.

 * @retval 0 Success, the rom is validated.
 * @retval !0 Generic failure, the rom is not validated, or the data is modified.
 */
int VPD_BP_Data_Validator(INOUT VPD_BP_Rom *rom);

/**
 * @fn int VPD_Full_Write(IN char* enc_sys_id, IN U8 *rom, IN int length, IN VPD_LOC vpd_pos)
 * @brief Write entire rom to assigned vpd eeprom.
 *
 * This function is called to write entire rom to assigned vpd eeprom.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[in] rom Pointer of the rom.
 * @param[in] length The length of the rom.
 * @param[in] vpd_pos Position of the VPD.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Full_Write(IN char *enc_sys_id, IN U8 *rom, IN int length, IN VPD_LOC vpd_pos);

/**
 * @fn int VPD_Full_Read(IN char* enc_sys_id, OUT U8 *rom, IN int length, IN VPD_LOC vpd_pos)
 * @brief Read entire rom from assigned vpd eeprom.
 *
 * This function is called to read entire rom from assigned vpd eeprom.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[out] rom Pointer of the rom.
 * @param[in] length The length of the rom.
 * @param[in] vpd_pos Position of the VPD.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Full_Read(IN char* enc_sys_id, OUT U8 *rom, IN int length, IN VPD_LOC vpd_pos);

/**
 * @fn void VPD_Print_Rom(OUT FILE *output, IN U8 *rom, IN int length)
 * @brief Print ROM content as hex value table.
 *
 * This function is called to print ROM content as hex value table.
 * @param[out] output Output file description (ex: stdout).
 * @param[in] rom The pointer of input rom.
 * @param[in] length The length of rom.
 */
void VPD_Print_Rom(OUT FILE *output, IN U8 *rom, IN int length);

/**
 * @fn int VPD_Gen_Default_ROM(IN VPD_LOC vpd_pos, OUT U8 *rom, IN int rom_size, IN char *file_name)
 * @brief Generate a rom file by default value.
 *
 * This function is called to generate a rom file by default value.
 * @param[in] vpd_pos The vpd postion.
 * @param[out] rom The pointer of the rom buffer.
 * @param[in] rom_size The size of the rom buffer.
 * @param[in] file_name Output file name.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Gen_Default_ROM(IN VPD_LOC vpd_pos, OUT U8 *rom, IN int rom_size, IN char *file_name);

/**
 * @defgroup VPD_Field_Access VPD Field Access function
 */

/**
 * @ingroup VPD_Field_Access
 */
/*@{*/  /* begin of VPD Field Access module */

//
// VPD Field Index
//
// VPD_LOC (2) | VPD_FIELD_TYPE (2) | VPD_FIELD_LEN (8) | VPD_FIELD_OFFSET (16)
//
#define VPD_FIELD_OFFSET_SHIFT			0
#define VPD_FIELD_OFFSET_SHIFT_SIZE		16
#define VPD_FIELD_LEN_SHIFT				(VPD_FIELD_OFFSET_SHIFT + VPD_FIELD_OFFSET_SHIFT_SIZE)
#define VPD_FIELD_LEN_SHIFT_SIZE		8
#define VPD_FIELD_TYPE_SHIFT			(VPD_FIELD_LEN_SHIFT + VPD_FIELD_LEN_SHIFT_SIZE)
#define VPD_FIELD_TYPE_SHIFT_SIZE		2
#define VPD_LOC_SHIFT					(VPD_FIELD_TYPE_SHIFT + VPD_FIELD_TYPE_SHIFT_SIZE)
#define VPD_LOC_SHIFT_SIZE				2

//
// VPD_FIELD_TYPE definition
//
#define VPD_FIELD_TYPE_STRING   0
#define VPD_FIELD_TYPE_BINARY   1
#define VPD_FIELD_TYPE_DATE     2

//
// VPD_FIELD_TYPE location definition
//
#define VPD_FIELD_TYPE_STRING_SHIFT	(VPD_FIELD_TYPE_STRING << VPD_FIELD_TYPE_SHIFT)
#define	VPD_FIELD_TYPE_BINARY_SHIFT	(VPD_FIELD_TYPE_BINARY << VPD_FIELD_TYPE_SHIFT)
#define	VPD_FIELD_TYPE_DATE_SHIFT	(VPD_FIELD_TYPE_DATE << VPD_FIELD_TYPE_SHIFT)

//
// VPD_LOC location definition
//
#define VPD_MB_SHIFT				(VPD_MB << VPD_LOC_SHIFT)
#define VPD_BP_SHIFT				(VPD_BP << VPD_LOC_SHIFT)
#define VPD_BP2_SHIFT				(VPD_BP2 << VPD_LOC_SHIFT)
#define VPD_BP3_SHIFT				(VPD_BP3 << VPD_LOC_SHIFT)

//
// VPD_FIELD_LENGTH location definition
//
#define VPD_FIELD_LEN(length)  (length << VPD_FIELD_LEN_SHIFT)

//
// VPD Field Index definition
//
#define VPD_NULL_INDEX                      0

#define VPD_MB_B_MFG_DATE_INDEX             (VPD_MB_SHIFT | VPD_FIELD_TYPE_DATE_SHIFT | VPD_FIELD_LEN(3) | 11)
#define VPD_MB_B_MANUFACTURER_INDEX         (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 15)
#define VPD_MB_B_PRODUCT_NAME_INDEX         (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 32)
#define VPD_MB_B_SERIAL_NUMBER_INDEX        (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 49)
#define VPD_MB_B_PART_NUMBER_INDEX          (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(32) | 66)
#define VPD_MB_B_SAS_ADDRESS_INDEX          (VPD_MB_SHIFT | VPD_FIELD_TYPE_BINARY_SHIFT | VPD_FIELD_LEN(16) | 100)
#define VPD_MB_P_MANUFACTURER_INDEX         (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 124)
#define VPD_MB_P_PRODUCT_NAME_INDEX         (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(32) | 141)
#define VPD_MB_P_PART_MODEL_NUMBER_INDEX    (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 174)
#define VPD_MB_P_PRODUCT_VERSION_INDEX      (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(3) | 191)
#define VPD_MB_P_SERIAL_NUMBER_INDEX        (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 195)
#define VPD_MB_P_NICKNAME_INDEX             (VPD_MB_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 214)

#define VPD_BP_C_TYPE_INDEX                 (VPD_BP_SHIFT | VPD_FIELD_TYPE_BINARY_SHIFT | VPD_FIELD_LEN(1) | 10)
#define VPD_BP_C_PART_NUMBER_INDEX          (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 12)
#define VPD_BP_C_SERIAL_NUMBER_INDEX        (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 29)
#define VPD_BP_B_MFG_DATE_INDEX             (VPD_BP_SHIFT | VPD_FIELD_TYPE_DATE_SHIFT | VPD_FIELD_LEN(3) | 51)
#define VPD_BP_B_MANUFACTURER_INDEX         (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 55)
#define VPD_BP_B_PRODUCT_NAME_INDEX         (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 72)
#define VPD_BP_B_SERIAL_NUMBER_INDEX        (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 89)
#define VPD_BP_B_PART_NUMBER_INDEX          (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(32) | 106)
#define VPD_BP_P_MANUFACTURER_INDEX         (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 148)
#define VPD_BP_P_PRODUCT_NAME_INDEX         (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 165)
#define VPD_BP_P_PART_MODEL_NUMBER_INDEX    (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 182)
#define VPD_BP_P_PRODUCT_VERSION_INDEX      (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(3) | 119)
#define VPD_BP_P_SERIAL_NUMBER_INDEX        (VPD_BP_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 203)

#define VPD_BP2_C_TYPE_INDEX                 (VPD_BP2_SHIFT | VPD_FIELD_TYPE_BINARY_SHIFT | VPD_FIELD_LEN(1) | 10)
#define VPD_BP2_C_PART_NUMBER_INDEX          (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 12)
#define VPD_BP2_C_SERIAL_NUMBER_INDEX        (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 29)
#define VPD_BP2_B_MFG_DATE_INDEX             (VPD_BP2_SHIFT | VPD_FIELD_TYPE_DATE_SHIFT   | VPD_FIELD_LEN(3) | 51)
#define VPD_BP2_B_MANUFACTURER_INDEX         (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 55)
#define VPD_BP2_B_PRODUCT_NAME_INDEX         (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 72)
#define VPD_BP2_B_SERIAL_NUMBER_INDEX        (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 89)
#define VPD_BP2_B_PART_NUMBER_INDEX          (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(32) | 106)
#define VPD_BP2_P_MANUFACTURER_INDEX         (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 148)
#define VPD_BP2_P_PRODUCT_NAME_INDEX         (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 165)
#define VPD_BP2_P_PART_MODEL_NUMBER_INDEX    (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 182)
#define VPD_BP2_P_PRODUCT_VERSION_INDEX      (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(3) | 119)
#define VPD_BP2_P_SERIAL_NUMBER_INDEX        (VPD_BP2_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 203)

#define VPD_BP3_C_TYPE_INDEX                 (VPD_BP3_SHIFT | VPD_FIELD_TYPE_BINARY_SHIFT | VPD_FIELD_LEN(1) | 10)
#define VPD_BP3_C_PART_NUMBER_INDEX          (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 12)
#define VPD_BP3_C_SERIAL_NUMBER_INDEX        (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 29)
#define VPD_BP3_B_MFG_DATE_INDEX             (VPD_BP3_SHIFT | VPD_FIELD_TYPE_DATE_SHIFT   | VPD_FIELD_LEN(3) | 51)
#define VPD_BP3_B_MANUFACTURER_INDEX         (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 55)
#define VPD_BP3_B_PRODUCT_NAME_INDEX         (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 72)
#define VPD_BP3_B_SERIAL_NUMBER_INDEX        (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 89)
#define VPD_BP3_B_PART_NUMBER_INDEX          (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(32) | 106)
#define VPD_BP3_P_MANUFACTURER_INDEX         (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 148)
#define VPD_BP3_P_PRODUCT_NAME_INDEX         (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 165)
#define VPD_BP3_P_PART_MODEL_NUMBER_INDEX    (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 182)
#define VPD_BP3_P_PRODUCT_VERSION_INDEX      (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(3) | 119)
#define VPD_BP3_P_SERIAL_NUMBER_INDEX        (VPD_BP3_SHIFT | VPD_FIELD_TYPE_STRING_SHIFT | VPD_FIELD_LEN(16) | 203)

/**
 * @fn int VPD_Get_Field_By_Id(IN char* enc_sys_id, IN char* vpd_field_id, OUT char* vpd_field_value, IN unsigned int vpd_field_len)
 * @brief Get single field info from vpd by field id.
 *
 * This function is called to get single field info from vpd.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[in] vpd_field_id It's a string of "vpd_pos:filed_no" pair, example: "0:3" means the third field of main board vpd.
 * @param[out] vpd_field_value The output buffer, output as ascii string.
 * @param[in] vpd_field_len The size of output buffer.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Get_Field_By_Id(IN char *enc_sys_id, IN char* vpd_field_id, OUT char* vpd_field_value, IN unsigned int vpd_field_len);

/**
 * @fn int VPD_Get_Field(IN char* enc_sys_id, IN unsigned int vpd_field_index, OUT char* vpd_field_value, IN unsigned int vpd_field_len)
 * @brief Get single field info from vpd by field index.
 *
 * This function is called to get single field info from vpd by field index.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[in] vpd_field_index Field Index, please check VPD Field Index definition.
 * @param[out] vpd_field_value The output buffer, output as ascii string.
 * @param[in] vpd_field_len The size of output buffer.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Get_Field(IN char* enc_sys_id, IN unsigned int vpd_field_index, OUT char* vpd_field_value, IN unsigned int vpd_field_len);

/**
 * @fn int VPD_Set_Field_By_Id(IN char* enc_sys_id, IN char* vpd_field_id, IN char* vpd_field_value)
 * @brief Set single field info to vpd.
 *
 * This function is called to get single field info from vpd.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[in] vpd_field_id It's a string of "vpd_pos:filed_no" pair, example: "0:3" means the third field of main board vpd.
 * @param[in] vpd_field_value The Input buffer, input as ascii string.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Set_Field_By_Id(IN char* enc_sys_id, IN char* vpd_field_id, IN char* vpd_field_value);

/**
 * @fn int VPD_Set_Field(IN char* enc_sys_id, IN unsigned int vpd_field_index, IN char* vpd_field_value)
 * @brief Set single field info from vpd by field index.
 *
 * This function is called to set single field info to vpd by field index.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @param[in] vpd_field_index Field Index, please check VPD Field Index definition.
 * @param[in] vpd_field_value The input buffer, input as ascii string.

 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Set_Field(IN char* enc_sys_id, IN unsigned int vpd_field_index, IN char* vpd_field_value);

/**
 * @fn int VPD_Get_Extend_Area(IN char *enc_sys_id, IN VPD_LOC vpd_pos, IN unsigned int vpd_offset, IN int vpd_length, OUT unsigned char* out_buf, IN int out_length)
 * @brief Read extended raw data (256-512bytes) from EEPROM attached on I2C bus
 *
 * This function will read extended raw data (256-512bytes) from EEPROM attached on I2C bus
 * @param[in] enc_sys_id system dependent enclosure identifier.
 * @param[in] vpd_pos Specified the I2C EEPROM attach where.
 * @param[in] vpd_offset offset from 0
 * @param[in] vpd_length raw data length
 * @param[out] out_buf raw data pointer
 * @param[in] out_length raw data max buffer length
 * @retval actual data read length.
 * @retval -1 Generic failure.
 */
int VPD_Get_Extend_Area(char *enc_sys_id, IN VPD_LOC vpd_pos, IN unsigned int vpd_offset, IN int vpd_length, OUT unsigned char* out_buf, IN int out_length);

/**
 * @fn int VPD_Set_Extend_Area(IN char *enc_sys_id, IN VPD_LOC vpd_pos, IN unsigned int vpd_offset, IN unsigned char* in_buf, IN int in_length)
 * @brief Write extended raw data (256-512bytes) to EEPROM attached on I2C bus.
 *
 * This function will write extended raw data (256-512bytes) to EEPROM attached on I2C bus.
 * @param[in] enc_sys_id system dependent enclosure identifier.
 * @param[in] vpd_pos Specified the I2C EEPROM attach where.
 * @param[in] vpd_offset offset from 0
 * @param[in] in_buf raw data length
 * @param[in] in_length raw data pointer
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int VPD_Set_Extend_Area(IN char *enc_sys_id, IN VPD_LOC vpd_pos, IN unsigned int vpd_offset, IN unsigned char* in_buf, IN int in_length);

/*@}*/  /* end of VPD Field Access module */
/*@}*/  /* end of VPD module */

/**
 * @defgroup EDID EDID function
 */

/**
 * @ingroup EDID EDID function
 */
/*@{*/  /* begin of EDID module */

/**
 * @fn int EDID_Get_Count(void)
 * @brief Get the EDID count in model.conf.
 *
 * This function is called to get the EDID count in model.conf.
 *
 * @retval number EDID count.
 */
int EDID_Get_Count(void);

/**
 * @fn int EDID_Read_and_Validate(IN int edid_id, OUT char *buf, IN int buf_size)
 * @brief Read EDID info and validate.
 *
 * This function is called to read EDID info and validate.
 *
 * @param[in] edid_id EDID id (0 based).
 * @param[out] buf The output buffer.
 * @param[in] buf_size The output buffer size.
 *
 * @retval 0 Success.
 * @retval -1 Validate failure.
 * @retval -2 Buffer size not enough.
 * @retval -3 EDID config is not present.
 * @retval -4 EDID config format is not correct.
 */
int EDID_Read_and_Validate(IN int edid_id, OUT char *buf, IN int buf_size);

/*@}*/  /* end of EDID module */

/**
 * @defgroup HAL_CONFIGURATION_FUNCTION HAL Configuration function
 */

/**
 * @ingroup HAL_CONFIGURATION_FUNCTION HAL Configuration function
 */
/*@{*/  /* begin of HAL Configuration module */

#define HAL_CONF_SETTING_FILE   "/etc/config/hal_setting.conf"
#define _DEV_NULL               " >&/dev/null"

#define HAL_CONF_SECTION_SSD                "SSD_SETTING"
#define HAL_CONF_FIELD_WARNED_SSD_LIFE_WWN  "ssd_warned_wwn_%s"

/**
 * @enum        HAL_DAEMON_NOTIFY
 * @brief       HAL Daemon Notify.
 */
typedef enum _HAL_DAEMON_NOTIFY
{
    HAL_NOTIFY_CONF_RELOAD = 1,     /*!< Reload hal daemon config. */
    HAL_NOTIFY_LAST,                /*!< Last one. */

} HAL_DAEMON_NOTIFY;

/**
 * @struct      HAL_CONF
 * @brief       HAL configuration structure.
 */
typedef struct _HAL_CONF
{
    int erc_read_secs;          /*!< ERC read seconds. */
	int erc_write_secs;         /*!< ERC write seconds. */
	int smart_interval_mins;    /*!< Get SMART information interval. */
} HAL_CONF;

/**
 * @fn int HAL_Daemon_Notify(IN HAL_DAEMON_NOTIFY reason, IN void* notify_contextP)
 * @brief Call hal_event to send a notify to hal_daemon.
 *
 * This function is called to make hal_daemon to do something, e.g, reload config... etc.
 * @param[in] reason Notify reason.
 * @param[in] notify_contextP The pointer of the input content.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Daemon_Notify(IN HAL_DAEMON_NOTIFY reason, IN void* notify_contextP);

/**
 * @fn int HAL_Conf_Get_All(INOUT HAL_CONF* hal_confP)
 * @brief Get all hal configuration.
 *
 * This function is called to get all hal configuration contents from config file /etc/config/hal_setting.conf.
 * @param[in] hal_confP The pointer of hal_conf structure.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_Get_All(INOUT HAL_CONF* hal_confP);

/**
 * @fn int HAL_Conf_ERC_Set(IN int read_secs, IN int write_secs)
 * @brief Set ERC configuration.
 *
 * @param[in] read_secs Read timeout value. -1 means no change, 0 means disable.
 * @param[in] write_secs Write timeout value. -1 means no change, 0 means disable.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_ERC_Set(IN int read_secs, IN int write_secs);

/**
 * @fn int HAL_Conf_ERC_Get(OUT int* read_secsP, OUT int* write_secsP)
 * @brief Set ERC configuration.
 *
 * @param[in] read_secsP The pointer of read timeout value.
 * @param[in] write_secsP The pointer of write timeout value.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_ERC_Get(OUT int* read_secsP, OUT int* write_secsP);

/**
 * @fn int HAL_Conf_SMART_Interval_Set(IN int interval_mins)
 * @brief Set the interval of get SMART information.
 *
 * This function is called to set the interval value to get SMART info from each disks.
 * @param[in] interval_mins The interval value. 0 means disable, interval is minute based.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_SMART_Interval_Set(IN int interval_mins);

/**
 * @fn int HAL_Conf_SMART_Interval_Get(OUT int* interval_minsP)
 * @brief Get the interval of get SMART information.
 *
 * This function is called to get the interval value to get SMART info from each disks.
 * The default value is 1 minute.
 * @param[in] interval_minsP The pointer of interval value.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_SMART_Interval_Get(OUT int* interval_minsP);

/**
 * @fn int HAL_Conf_UASP_Set(IN int enable)
 * @brief Set the UASP status.
 *
 * This function is called to set UASP status by insert or remove module uas.ko.
 * Please check the return value, it's possible to remove module fail due to device lock.
 * and this should be handle by caller.
 * @param[in] enable 0 means disable, 1 means enable.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_UASP_Set(IN int enable);

/**
 * @fn int HAL_Conf_UASP_Get(OUT int* enableP)
 * @brief Get the UASP status.
 *
 * This function is called to get UASP status from config file.
 * @param[in] enableP 0 means disable, 1 means enable.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int HAL_Conf_UASP_Get(OUT int* enableP);

/*@}*/  /* end of HAL Configuration module */

/**
 * @defgroup LIB Resource Control function
 */
/**
 * @ingroup LIB Resource Control function
 */
/*@{*/  /* begin of resctrl module */

/**
 * @fn int Swap_File_Feature_Support(IN int skip_cgroup_check)
 * @brief Check support file-based swap features
 *
 * This function is used to check file-based swap support
 * skip_cgroup_check will skip check memory.qnap_rss_in_bytes.
 * Only hal_daemon can skip cgroup check,
 * other application please so cgroup check.
 *
 * @param[in] skip_cgroup_check Skip cgroup check, only set to 1 when hal_daemon start up.
 * @retval 1 Support.
 * @retval 0 Not support.
 */
int Swap_File_Feature_Support(IN int skip_cgroup_check);

/**
 * @fn int Swap_File_Is_Enable(IN char *file)
 * @brief Check file is in swap or not
 *
 * This function is used to check file in swap or not
 *
 * @param[in] file File name to be checked.
 * @retval 1 In swap.
 * @retval 0 Not in swap.
 */
int Swap_File_Is_Enable(IN char *file);

/**
 * @fn int Swap_File_Create(IN char *file, IN unsigned long long size)
 * @brief Create a file and add into swap
 *
 * This function is used to create file and add into swap.
 * If file exist, we will remove it first.
 * Do not use this function except hal_daemon
 *
 * @param[in] file File name to be create.
 * @param[in] size File size for created file, in GB.
 * @retval 0 Success.
 * @retval 1 Already created.
 * @retval Otherwise, generic failure.
 */
int Swap_File_Create(char *file, unsigned long long size);

/**
 * @fn int Swap_File_Destroy(IN char *file)
 * @brief Remove swap file and delete it.
 *
 * This function is used to remove file from swap and delete it.
 * Do not use this function except hal_daemon
 *
 * @param[in] file File name to be remove.
 * @retval 0 Success.
 * @retval 1 Already Destroy.
 * @retval Otherwise, generic failure.
 */
int Swap_File_Destroy(IN char *file);

/**
 * @fn int Mem_Ctrl_Init(void)
 * @brief Mount memory cgroup .
 *
 * This function is used mount cgroup for hal_daemon.
 *
 * @retval 0 Success
 * @retval Otherwise, generic failure.
 */
int Mem_Ctrl_Init(void);

/**
 * @fn int Mem_Wait_Threshold(IN char *control_file, IN char *usage_file, unsigned long long threshold_in_bytes, IN int is_high_threshold)
 * @brief Wait for eventfd by threshold
 *
 * This function is used to wait eventfd.
 * Also check memory is over or under threshold according high or low threshold
 * before wait threshold-cross event.
 * And this function will block until eventfd signal occured.
 * If you want to interrupt it, please set alarm or other signal.
 *
 * @param[in] control_file Cgroup control file name.
 * @param[in] usage_file Cgroup usage file name.
 * @param[in] threshold_in_bytes Threshold in bytes.
 * @param[in] is_high_threshold Threshold is high or low threshold.
 * @retval 1 Eventfd notifiered
 * @retval 0 Interrupt by signal
 * @retval Otherwise, generic failure.
 */
int Mem_Wait_Threshold(IN char *control_file, IN char *usage_file, IN unsigned long long threshold_in_bytes, IN int is_high_threshold);
/*@}*/  /* end of resctrl module */

/**
 *  @defgroup IPMI Inband Management Function
 */

/**
 *  @ingroup IPMI Inband Management Function
 */

/*@{*/    /* begin of IPMI inband management module */

#define IPMI_MAX_USERNAME    33    /*!< Buffer length of username credential, including the trailing zero */
#define IPMI_MAX_PASSWORD    33    /*!< Buffer length of Password credential, including the trailing zero */
#define IPMI_MAX_MAC_LEN    18    /*!< Buffer length of MAC address in "XX:XX:XX:XX:XX:XX" format, including the trailing zero */
#define IPMI_MAX_IPV4_LEN    16    /*!< Buffer length of IPV4 address in "XXX.XXX.XXX.XXX" format, including the trailing zero */
#define IPMI_MAX_IPV6_LEN    64    /*!< Buffer length of IPV6 address, including the trailing zero */

/**
 * @enum    IPMI_LINK_STATUS
 * @brief    Link status of IPMI management port.
 */
typedef enum _IPMI_LINK_STATUS
{
    IPMI_LINK_DOWN,                /*!< link down status */
    IPMI_LINK_UP,                /*!< link up status */
} IPMI_LINK_STATUS;

/**
 * @enum    IPMI_NETWORK_SPEED
 * @brief    Network speed of IPMI management port.
 */
typedef enum _IPMI_NETWORK_SPEED
{
    IPMI_NETWORK_SPEED_AUTO,                /*!< network speed auto negotiate */
    IPMI_NETWORK_SPEED_10M,                /*!< network speed 10M */
    IPMI_NETWORK_SPEED_100M,              /*!< network speed 100M */
    IPMI_NETWORK_SPEED_1G,                 /*!< network speed 1G */
} IPMI_NETWORK_SPEED;

/**
 * @enum    IPMI_IP_CONFIG
 * @brief    IP configuration type of IPMI management port.
 */
typedef enum _IPMI_IP_CONFIG
{
    IPMI_IP_DHCP,                /*!< Use DHCP to obtain the IP address */
    IPMI_IP_STATIC,                /*!< Assign IP manually */
    IPMI_IPAUTO_CONF,            /*!< Use zero configuration */
} IPMI_IP_CONFIG;

/**
 * @enum    IPMI_RESTORE_STATUS
 * @brief    IPMI restore process status.
 */
typedef enum _IPMI_RESTORE_STATUS
{
    IPMI_RESTORE_PROCESSING,                /*!< IPMI restore process is processing */
    IPMI_RESTORE_SUCCESS,                   /*!< IPMI restore process is success */
    IPMI_RESTORE_FAIL,                    /*!< IPMI restore process is failed */
} IPMI_RESTORE_STATUS;

/**
 * @struct    IPMI_CREDENTIAL
 * @brief    IPMI management credential.
 */
typedef struct _IPMI_CREDENTIAL
{
    char username[IPMI_MAX_USERNAME];        /*!< User name of IPMI administrator */
    char password[IPMI_MAX_PASSWORD];        /*!< Password of IPMI administrator */
} IPMI_CREDENTIAL;

/**
 * @struct    IPMI_NET_INFO
 * @brief    Network information of IPMI management port.
 */
typedef struct _IPMI_NET_INFO
{
    int net_enable;                             /*!< Represent network management port enable (1) or not (0) */
    IPMI_NETWORK_SPEED network_speed;            /*!< Network speed of network management port*/
    IPMI_LINK_STATUS link_status;               /*!< Link status of network management port */
    char hwaddr[IPMI_MAX_MAC_LEN];              /*!< MAC address of network management port */

    /* IPMI IPV4 Settings */
    IPMI_IP_CONFIG ipv4_config;                 /*!< IPV4 configuration type */
    char ipv4_addr[IPMI_MAX_IPV4_LEN];          /*!< IPV4 address */
    char ipv4_netmask[IPMI_MAX_IPV4_LEN];       /*!< IPV4 network mask */
    char ipv4_gateway[IPMI_MAX_IPV4_LEN];       /*!< IPV4 gateway address */

    /* IPMI IPV6 Settings */
    int ipv6_enable;                            /*!< Represent ipv6 settings of network management port enable (1) or not (0) */
    IPMI_IP_CONFIG ipv6_config;                 /*!< IPV6 configuration type */
    char ipv6_addr[IPMI_MAX_IPV6_LEN];          /*!< IPV6 address */
    char ipv6_gateway[IPMI_MAX_IPV6_LEN];       /*!< IPV6 gateway address */
    unsigned int prefix_len;                    /*!< IPV6 address prefix length */
} IPMI_NET_INFO;

/**
 * @fn int IPMI_Is_Available()
 * @brief Check if the IPMI module is installed.
 *
 * This function is called by IPMI management facility to check if the IPMI module is installed or not.
 * @retval 1 True, The IPMI module is installed and available to use.
 * @retval 0 False, The IPMI module is not installed.
 */
int IPMI_Is_Available();

/**
 * @fn int IPMI_Restore_Factory_Default(IN IPMI_CREDENTIAL *credentialP)
 * @brief Reset the IPMI module to the firmware default configuration.
 *
 * This function is called by IPMI management facility to reset the IPMI module configuration as factory default settings.
 * @param[in] credentialP IPMI_CREDENTIAL structure pointer, point to the IPMI management credential.
 * @retval 0 Success, Send reset request to IPMI module successfully.
 * @retval -1 Generic failure, Fail to reset IPMI module settings.
 */
int IPMI_Restore_Factory_Default(IN IPMI_CREDENTIAL *credentialP);

/**
 * @fn int IPMI_Get_Restore_Status(IN IPMI_CREDENTIAL *credentialP)
 * @brief Check the IPMI restore process status.
 *
 * This function is called by IPMI management facility to check the IPMI restore process is done or is processing.
 * @param[in] credentialP IPMI_CREDENTIAL structure pointer, point to the IPMI management credential.
 * @retval IPMI_RESTORE_PROCESSING Processing, Reset IPMI module still processing.
 * @retval IPMI_RESTORE_SUCCESS Success, Reset IPMI module process is success.
 * @retval IPMI_RESTORE_FAIL Generic failure, Rest IPMI module process is failed or timeout.
 */
IPMI_RESTORE_STATUS IPMI_Get_Restore_Status(IN IPMI_CREDENTIAL *credentialP);

/**
 * @fn int IPMI_Net_Get_Info(IN IPMI_CREDENTIAL *credentialP, OUT IPMI_NET_INFO *infoP)
 * @brief Retrieve current settings of IPMI network management port.
 *
 * This function is called by IPMI management facility to obtain the current network settings
 * of IPMI management port.
 * @param[in] credentialP IPMI_CREDENTIAL structure pointer, point to the IPMI management credential.
 * @param[out] infoP IPMI_NET_INFO structure pointer to host current network settings.
 * @retval 0 Success, current network settings is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve current network settings.
 */
int IPMI_Net_Get_Info(IN IPMI_CREDENTIAL *credentialP, OUT IPMI_NET_INFO *infoP);

/**
 * @fn int IPMI_Net_Set_Info(IN IPMI_CREDENTIAL *credentialP, IN IPMI_NET_INFO *infoP)
 * @brief Change settings of IPMI network management port.
 *
 * This function is called by IPMI management facility to apply new network settings.
 * @param[in] credentialP IPMI_CREDENTIAL structure pointer, point to the IPMI management credential.
 * @param[out] infoP IPMI_NET_INFO structure pointer to host new network settings.
 * @retval 0 Success, new network settings is applied completely.
 * @retval -1 Generic failure, Fail to apply new network settings.
 */
int IPMI_Net_Set_Info(IN IPMI_CREDENTIAL *credentialP, IN IPMI_NET_INFO *infoP);

/*@}*/    /* end of IPMI inband management module */

/**
 *  @defgroup Secure Erase Function
 */

/**
 *  @ingroup Secure Erase Function
 */

/*@{*/    /* begin of Secure Erase Function */

#define SECURE_ERASE_CONF_FILE             "/etc/config/secure_erase.conf" /*!< Secure erase config file path */
#define SECURE_ERASE_CONF_PID_KEY          "pid"                    /*!< Secure erase config key of pid */
#define SECURE_ERASE_CONF_MODE_KEY         "mode"                   /*!< Secure erase config key of mode */
#define SECURE_ERASE_CONF_PROGRESS_KEY     "progress"               /*!< Secure erase config key of progress */
#define SECURE_ERASE_CONF_STATUS_KEY       "status"                 /*!< Secure erase config key of status */
#define SECURE_ERASE_CONF_FORCE_KEY        "force"                 /*!< Secure erase config key of NAS force */
#define SECURE_ERASE_EVENT_STARTED_LOG     "[Secure Erase] Secure Erase %s started."           /*!< Secure erase event log for started */
#define SECURE_ERASE_EVENT_COMPLETED_LOG   "[Secure Erase] Secure Erase %s completed."         /*!< Secure erase event log for completed */
#define SECURE_ERASE_EVENT_CANCELED_LOG    "[Secure Erase] Secure Erase %s canceled manually." /*!< Secure erase event log for canceled */
#define SECURE_ERASE_EVENT_FORCE_CANCELED_LOG    "[Secure Erase] Secure Erase %s canceled."          /*!< Secure erase event log for canceled */
#define SECURE_ERASE_EVENT_INCOMPLETED_LOG "[Secure Erase] Secure Erase %s incompleted, please make sure the disk is correctly plugged in and conduct Secure Erase again." /*!< Secure erase event log for frozen */
#define SECURE_ERASE_EVENT_FROZEN_LOG      "[Secure Erase] Secure Erase %s cannot be start, please re-plug in the disk and conduct Secure Erase again." /*!< Secure erase event log for canceled */
#define SECURITY_USER_PASSWORD             "QNAP"        /*!< ATA security user password */
#define SECURITY_USER_PASSWORD_SIZE        32            /*!< ATA size of security user password */
#define SECURITY_STATUS_SUPPORTED          0x01          /*!< ATA security supported bit */
#define SECURITY_STATUS_ENABLED            0x02          /*!< ATA security enabled bit */
#define SECURITY_STATUS_LOCKED             0x04          /*!< ATA security locked bit */
#define SECURITY_STATUS_FROZEN             0x08          /*!< ATA security frozen bit */
#define OVERWRITE_SIZE                     (1024 * 1024) /*!< Buffer length of write size */
#define OVERWRITE_UPDATE_COUNT             200           /*!< Update overwrite config file count */

/**
 * @enum    SECURE_ERASE_STATUS
 * @brief    Secure erase status.
 */
typedef enum _SECURE_ERASE_STATUS
{
    SECURE_ERASE_IDLE = 0,    /*!< Secure erase status is idle by initialization state */
    SECURE_ERASE_COMPLETED,   /*!< Secure erase status is completed by PD_Secure_Erase_Start() finished */
    SECURE_ERASE_INCOMPLETED, /*!< Secure erase status is incompleted by PD_Secure_Erase_Start() failure */
    SECURE_ERASE_PROCESS,     /*!< Secure erase status is process by PD_Secure_Erase_Start() */
    SECURE_ERASE_CANCELED,    /*!< Secure erase status is canceled by PD_Secure_Erase_Canceled() successfully */
    SECURE_ERASE_FROZEN,      /*!< Secure erase status is frozen, which need to replug-in device */
} SECURE_ERASE_STATUS;

/**
 * @enum    SECURE_ERASE_MODE
 * @brief    Secure erase select mode.
 */
typedef enum _SECURE_ERASE_MODE
{
    SECURE_ERASE_FAST, /*!< Fast erase mode, which execute overwrite partition table */
    SECURE_ERASE_SSD,  /*!< SSD erase mode, which execute security erase via ATA command */
    SECURE_ERASE_FULL, /*!< Completed erase mode, which execute overwrite */
    SECURE_ERASE_SED,  /*!< Instant secure erase mode, which execute seagate erase */
} SECURE_ERASE_MODE;

/**
 * @enum    SECURE_ERASE_PATTERN
 * @brief    Secure erase select pattern.
 */
typedef enum _SECURE_ERASE_PATTERN
{
    SECURE_ERASE_ZERO = 0, /*!< Secure erase pattern is zero */
    SECURE_ERASE_ONE,      /*!< Secure erase pattern is one */
    SECURE_ERASE_RAND,     /*!< Secure erase pattern is random */
} SECURE_ERASE_PATTERN;

/**
 * @struct    SECURE_ERASE_INFO
 * @brief      Erase count and erase pattern for overwrite data.
 */
typedef struct _SECURE_ERASE_INFO
{
    int count;                    /*!< Erase count */
    int pattern[MAX_ERASE_COUNT]; /*!< Erase pattern for each count */
} SECURE_ERASE_INFO;

/**
 * @struct    MBR_FORMAT
 * @brief      Dos partition table format.
 */
typedef struct _MBR_FORMAT
{
    char boot_code_1;                 /*!< Boot code */
    char boot_code_2_446[445];        /*!< Boot code */
    char partition_table_entry[16*4]; /*!< Partition table entry 1~4 */
    char signature_value[2];          /*!< Signature value */
} __attribute__((packed)) MBR_FORMAT;

/**
 * @struct    PRIMARY_GPT_FORMAT
 * @brief      GUID partition table format.
 */
typedef struct _PRIMARY_GPT_FORMAT
{
    char gpt_header_1;                   /*!< Primary gpt header */
    char gpt_header_2_511[511];          /*!< Primary gpt header */
    char partition_table_entry[128*128]; /*!< Partition table entry 1~128 */
} __attribute__((packed)) PRIMARY_GPT_FORMAT;

/**
 * @struct    SECONDARY_GPT_FORMAT
 * @brief      Backup GUID partition table format.
 */
typedef struct _SECONDARY_GPT_FORMAT
{
    char partition_table_entry_1;                /*!< Partition table entry 1 of one byte */
    char partition_table_entry_2_128[128*128-1]; /*!< Partition table entry 2~128 and entry 1 fo 127 byte */
    char gpt_header[512];                        /*!< Secondary gpt header */
} __attribute__((packed)) SECONDARY_GPT_FORMAT;

/**
 * @fn int PD_Secure_Erase_Support(IN int enc_id, IN int port_id, IN SECURE_ERASE_MODE mode)
 * @brief Support selection mode or not.
 *
 * This function is called by nas utility to check the specified physical disk support.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode Select secure erase mode.
 * @retval 1 Success, the selection mode support.
 * @retval 0 Success, the selection mode not support.
 * @retval -1 Generic faiure, incomplete the specified action.
 */
int PD_Secure_Erase_Support(IN int enc_id, IN int port_id, IN SECURE_ERASE_MODE mode);

/**
 * @fn int PD_Secure_Erase_Start(IN int enc_id, IN int port_id, IN SECURE_ERASE_MODE mode, IN SECURE_ERASE_INFO info)
 * @brief Secure erase the specified physical disk.
 *
 * This function is called by nas utility to secure erase the specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode Select secure erase mode.
 * @param[in] info Erase count and erase pattern only for overwrite mode.
 * @retval 0 Success, the specified action(fork process) is started successfully.
 * @retval -1 Generic failure(ex: processing), fail to start the specified action.
 */
int PD_Secure_Erase_Start(IN int enc_id, IN int port_id, IN SECURE_ERASE_MODE mode, IN SECURE_ERASE_INFO info);

/**
 * @fn int PD_Secure_Erase_Get_Progress(IN int enc_id, IN int port_id, OUT float* progressP, OUT SECURE_ERASE_STATUS* statusP)
 * @brief Retrieve the specified secure erase progress of a physical disk.
 *
 * This function is called by nas utility to obtain the specified progress and status of a physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] progressP a pointer to store returned progress.
 * @param[out] statusP a pointer to store returned status.
 * @retval 0 Success, the specified progress is retrieved completely.
 * @retval -1 Generic failure, fail to retrieve the specified progress.
 */
int PD_Secure_Erase_Get_Progress(IN int enc_id, IN int port_id, OUT float* progressP, OUT SECURE_ERASE_STATUS* statusP);

/**
 * @fn int PD_Secure_Erase_Cancel(IN int enc_id, IN int port_id)
 * @brief Stop the secure erase operation of a specified physical disk.
 *
 * This function is called by nas utility to cancel the secure erase operation of a specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 0 Success, the specified action is cancelled successfully.
 * @retval -1 Generic failure, fail to cancel the specified action.
 */
int PD_Secure_Erase_Cancel(IN int enc_id, IN int port_id);

/*@}*/    /* end of Secure Erase Function */

/**
 *  @defgroup Endurance Estimate Function
 */

/**
 *  @ingroup Endurance Estimate Function
 */

/*@{*/    /* begin of Endurance Estimate Function */

#define ENDURANCE_ESTIMATE_CONF_FILE                "/etc/config/ssd_endurance_record.conf" /*!< Endurance estimate config file path. */
#define ENDURANCE_ESTIMATE_CONF_STATUS_KEY          "status"                                /*!< Endurance estimate config key of status. */
#define ENDURANCE_ESTIMATE_CONF_VENDOR_KEY          "vendor"                                /*!< Endurance estimate config key of vendor. */
#define ENDURANCE_ESTIMATE_CONF_MODEL_KEY           "model"                                 /*!< Endurance estimate config key of model. */
#define ENDURANCE_ESTIMATE_CONF_SERIAL_NO_KEY       "serial_no"                             /*!< Endurance estimate config key of serial number. */
#define ENDURANCE_ESTIMATE_CONF_SECTOR_COUNT_KEY    "sector_count"                          /*!< Endurance estimate config key of sector write count. */
#define ENDURANCE_ESTIMATE_CONF_TOTAL_WRITTEN_KEY   "total_written"                         /*!< Endurance estimate config key of total written of bytes. */
#define ENDURANCE_ESTIMATE_CONF_TOTAL_DAY_KEY       "total_day"                             /*!< Endurance estimate config key of total written of days. */
#define ENDURANCE_ESTIMATE_CONF_AVERAGE_WRITTEN_KEY "average_written"                       /*!< Endurance estimate config key of average written of bytes. */
#define ENDURANCE_ESTIMATE_CONF_RECORD_START_KEY    "record_start"                          /*!< Endurance estimate config key of record date. */
#define ENDURANCE_ESTIMATE_CONF_POWER_ON_HOURS_KEY  "power_on_hours"                        /*!< Endurance estimate config key of power on hours. */
#define ENDURANCE_ESTIMATE_CONF_TBW_KEY             "TBW"                                   /*!< Endurance estimate config key of TeraBytes Written. */
#define ENDURANCE_ESTIMATE_CONF_WRITTEN_LOG_KEY     "written_log"                           /*!< Endurance estimate config key of written log. */
#define ENDURANCE_ESTIMATE_CONF_LIFE_PERCENT_KEY    "life_percent"                          /*!< Endurance estimate config key of life percnet. */
#define ENDURANCE_ESTIMATE_CONF_LIFE_REMAIN_KEY     "life_remain"                           /*!< Endurance estimate config key of life remain. */
#define ENDURANCE_ESTIMATE_STATUS_EXIST           0x00000001 /*!< Endurance estimate record exist bit which represent to record exist or not. */
#define ENDURANCE_ESTIMATE_STATUS_WRITTEN_RECORD  0x00000002 /*!< Written record enable bit which represent to record daily written or not. */
#define ENDURANCE_ESTIMATE_STATUS_LIFE_ESTIMATION 0x00000004 /*!< Endurance estimate enable bit which represent to show life dashboard or not. */

/**
 * @struct    ENDURANCE_ESTIMATE_INFO
 * @brief     Endurance estimate information of SSD.
 */
typedef struct _ENDURANCE_ESTIMATE_INFO
{
    int status;                         /*!< Endurance estimate capability bits support or enable. */
    char vendor[MAX_VENDOR_LEN];        /*!< Vendor name. */
    char model[MAX_MODEL_LEN];          /*!< Model name. */
    char serial_no[MAX_SERIAL_NO_LEN];  /*!< Disk unit serial number. */
    unsigned long long sector_count;    /*!< Latest sector write count. */
    unsigned long long total_written;   /*!< Current written of bytes. */
    int total_day;                      /*!< Current written of days. */
    unsigned long long avg_written;     /*!< Current average written of bytes. */
    char record_start[MAX_DATE_LEN];    /*!< Start of record date. */
    long long power_on_hours;           /*!< Power on hours, the value = -1 represent not support. */
    unsigned long long tbw;             /*!< Expect TeraBytes Written. */
    unsigned long long written_log;     /*!< Written log of bytes. */
    int life_percent;                   /*!< Remain percentage, the value = -1 represent not support. */
    long long life_remain;              /*!< Estimate remain life of days, the value = -1 represent not support. */
} ENDURANCE_ESTIMATE_INFO;

/**
 * @fn int PD_Endurance_Estimate_Set_Conf(IN char *section_name, IN ENDURANCE_ESTIMATE_INFO* info)
 * @brief set endurance estimate the specified physical disk config file.
 *
 * This function is called by set config file.
 * @param[in] section_name Section name.
 * @param[in] info Write infomation.
 * @retval 0 Success, the specified action is set successfully.
 * @retval -1 Generic failure, fail to set the specified action.
 */
int PD_Endurance_Estimate_Set_Conf(IN char *section_name, IN ENDURANCE_ESTIMATE_INFO* info);

/**
 * @fn int PD_Endurance_Estimate_Search_Record(IN int enc_id, IN int port_id)
 * @brief Search the written record of a specified physical disk.
 *
 * This function is called by nas utility to search the written record of a specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 Success, search found.
 * @retval 0 Success, search not found.
 * @retval -1 Generic failure, fail to search the specified action.
 */
int PD_Endurance_Estimate_Search_Record(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Endurance_Estimate_Get_Count(IN int enc_id, IN int port_id, OUT unsigned long long* count)
 * @brief Get the sector write count of a specified physical disk via /proc/diskstats.
 *
 * This function is called by nas utility to get the sector write count  of a specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] count Sector write count.
 * @retval 0 Success, the specified action is get sectors write count successfully.
 * @retval -1 Generic failure, fail to get written the specified action.
 */
int PD_Endurance_Estimate_Get_Count(IN int enc_id, IN int port_id, OUT unsigned long long* count);

/**
 * @fn int PD_Endurance_Estimate_Get_Info(IN int enc_id, IN int port_id, OUT ENDURANCE_ESTIMATE* infoP)
 * @brief Retrieve the specified endurance estimate information of a physical disk.
 *
 * This function is called by nas utility to obtain the specified infotmation of a physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] infoP a pointer to store returned enducance estimate infotmation.
 * @retval 0 Success, the specified information is retrieved completely.
 * @retval -1 Generic failure, fail to retrieve the specified information.
 */
int PD_Endurance_Estimate_Get_Info(IN int enc_id, IN int port_id, OUT ENDURANCE_ESTIMATE_INFO* infoP);

/**
 * @fn int PD_Endurance_Estimate_Set_Enable(IN int enc_id, IN int port_id, IN int mode, IN int enable)
 * @brief Set the feature about endurance estimate of a specified physical disk.
 *
 * This function is called by nas utility to set the endurance estimate feature of a specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode Specified feature, 0->written record, 1->life estimation.
 * @param[in] enable Enable or not, 0->disable, 1->enable.
 * @retval 0 Success, the specified action is set successfully.
 * @retval -1 Generic failure, fail to set the specified action.
 */
int PD_Endurance_Estimate_Set_Enable(IN int enc_id, IN int port_id, IN int mode, IN int enable);

/**
 * @fn int PD_Endurance_Estimate_Clear_Record(IN int enc_id, IN int port_id)
 * @brief Clear the written record of a specified physical disk.
 *
 * This function is called by nas utility to clear the written record of a specified physical disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_Endurance_Estimate_Clear_Record(IN int enc_id, IN int port_id);

/**
 * @fn int PD_Get_USB_Port_Id(IN char* usb_bus, OUT int* port_id)
 * @brief Get usb port_id by usb_bus.
 *
 * This function is called to get usb port_id by usb_usb.
 * @param[in] usb_bus usb bus of usb device.
 * @param[out] port_id port identifier.
 * @retval 0 Success, get usb port_id successfully.
 * @retval -1 Generic failure.
 */
int PD_Get_USB_Port_Id(IN char* usb_bus, OUT int* port_id);

/**
 * @fn int PD_Get_ODD_Owner(IN int enc_id, IN int port_id, OUT char* owner, IN int buf_size)
 * @brief Get ODD owner.
 *
 * This function is called to get ODD owner by enc_id and port_id.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] owner Return owner string if not be set to NULL.
 * @param[in] buf_size Buffer size of string pointer.
 * @retval 0 Owner is QTS.
 * @retval 1 Owner is other application.
 */
int PD_Get_ODD_Owner(IN int enc_id, IN int port_id, OUT char* owner, IN int buf_size);

/**
 * @fn int PD_ATA_ReadWrite_Buffer_Command_Supportted(IN int enc_id, IN int port_id)
 * @brief Support ATA Pass Through command Write/Read Buffer or not
 *
 * This function is called by nas utility to check if the specified disk supports ATA Pass Through command Write/Read Buffer or not
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 Success, command is supportted.
 * @retval 0 Success, command is not supportted.
 */
int PD_ATA_ReadWrite_Buffer_Command_Supportted(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ATA_ReadWrite_Buffer_DMA_Command_Supportted(IN int enc_id, IN int port_id)
 * @brief Support ATA Pass Through command Write/Read Buffer DMA or not
 *
 * This function is called by nas utility to check if the specified disk supports ATA Pass Through command Write/Read Buffer DMA or not
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 1 Success, command is supportted.
 * @retval 0 Success, command is not supportted.
 */
int PD_ATA_ReadWrite_Buffer_DMA_Command_Supportted(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ATA_Write_Buffer(IN int enc_id, IN int port_id, IN int mode)
 * @brief Send a ata write buffer pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata write buffer pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode The written pattern type; mode=0->random, 1->fixed pattern 0x5a, 2->sequentially write 0x00 to 0xff
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Write_Buffer(IN int enc_id, IN int port_id, IN int mode);

/**
 * @fn int PD_ATA_Read_Buffer(IN int enc_id, IN int port_id)
 * @brief Send a ata read buffer pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata read buffer pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Read_Buffer(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ATA_Compare_WriteRead_Buffer(IN int enc_id, IN int port_id, IN int mode, IN unsigned long long round)
 * @brief Send a ata write buffer pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata write buffer pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode The written pattern type; mode=0->random, 1->fixed pattern 0x5a, 2->sequentially write 0x00 to 0xff
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Compare_WriteRead_Buffer(IN int enc_id, IN int port_id, IN int mode, IN unsigned long long round);

/**
 * @fn int PD_ATA_Write_Buffer_DMA(IN int enc_id, IN int port_id, IN int mode)
 * @brief Send a ata write buffer dma pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata write dma buffer pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode The written pattern type; mode=0->random, 1->fixed pattern 0x5a, 2->sequentially write 0x00 to 0xff
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Write_Buffer_DMA(IN int enc_id, IN int port_id, IN int mode);

/**
 * @fn int PD_ATA_Read_Buffer_DMA(IN int enc_id, IN int port_id)
 * @brief Send a ata read buffer dma pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata read buffer dma pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Read_Buffer_DMA(IN int enc_id, IN int port_id);

/**
 * @fn int PD_ATA_Compare_WriteRead_Buffer_DMA(IN int enc_id, IN int port_id, IN int mode, IN unsigned long long round)
 * @brief Send a ata write buffer pass through command to the specified disk.
 *
 * This function is called by nas utility to send a ata write buffer pass through command to the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[in] mode The written pattern type; mode=0->random, 1->fixed pattern 0x5a, 2->sequentially write 0x00 to 0xff
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_ATA_Compare_WriteRead_Buffer_DMA(IN int enc_id, IN int port_id, IN int mode, IN unsigned long long round);

/**
 * @fn int PD_Get_Negotiation_Link_Rate(IN int enc_id, IN int port_id, OUT PD_LINK_SPEED *curr_speed)
 * @brief Get the negotiation link speed of the specified disk.
 *
 * This function is called by nas utility to get the negotiation link speed of the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @param[out] curr_speed Negotiation speed.
 * @retval 0 Success, the specified action is cleared successfully.
 * @retval -1 Generic failure, fail to clear the specified action.
 */
int PD_Get_Negotiation_Link_Rate(IN int enc_id, IN int port_id, OUT PD_LINK_SPEED *curr_speed);

/**
 * @fn int PD_Get_Data_Package(IN int enc_id, IN int port_id)
 * @brief Get the data package of the specified disk.
 *
 * This function is called by nas utility to get the data package of the specified disk.
 * @param[in] enc_id Logical enclosure identifier.
 * @param[in] port_id Physical port id assigned by system dependent layer.
 * @retval 0 Success, the specified action is done successfully.
 * @retval -1 Generic failure, fail to get the data package.
 */
int PD_Get_Data_Package(IN int enc_id, IN int port_id);

/**
 * @fn int SE_Get_LCM_Baud_Type(IN int enc_id, OUT int *type)
 * @brief Get LCM module baudrate type from model.conf
 *
 * This function is called to get LCD module baudrate in
 * the form defined in termios.h from model.conf
 * @param[in] enc_id Enclosure identifier (0 based).
 * @param[out] baud_type baudrate type.
 * @retval 0 Success, get baudrate type successfully.
 * @retval -1 Generic failure.
 */
int SE_Get_LCM_Baud_Type(IN int enc_id, OUT int *type);

/*@}*/    /* end of Endurance Estimate Function */

/**
 *  @defgroup MTP Function
 */

/**
 *  @ingroup MTP Function
 */

/*@{*/    /* begin of MTP Function */

// MTP config files
#define MTP_HAL_CONF    "/etc/mtp_hal.conf"
#define MTP_RUN_CONF    "/etc/mtp_run.conf"

// MTP other defines
#define MTP_MNT_PATH_LENGTH     64
#define MTP_SYS_ID_LENGTH       16
#define MTP_STORAGE_NAME_LENGTH 64

/**
 * @fn          typedef int (*MTP_ENUM_CALLBACK) (IN int mtp_id, IN void* contextP)
 * @brief       The function prototpye of MTP enumeration callback.
 * @param[in]   mtp_id Logical MTP device identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 * @retval      -2 Skip, callback function want to skip this identifier combination.
 */
typedef int (*MTP_ENUM_CALLBACK) (IN int mtp_id, IN void* contextP);

/**
 * @struct      MTP_INFO
 * @brief       MTP Information.
 */
typedef struct _MTP_INFO {
    int mtp_id;                         /*!< MTP logical ID for MTP related API. */
    char mtp_sys_id[MTP_SYS_ID_LENGTH]; /*!< MTP Physical ID, Ex: "usb.001.015". */
    char vendor[MAX_VENDOR_LEN];        /*!< Vendor name. */
    char model[MAX_MODEL_LEN];          /*!< Model name. */
    char serial_no[MAX_SERIAL_NO_LEN];  /*!< Serial number. */
    unsigned long long total_size;      /*!< Storage total size. (will be removed)*/
    unsigned long long used_size;       /*!< Storage used size. (will be removed) */
    unsigned long long free_size;       /*!< Storage free size. (will be removed) */
    char mnt_path[MTP_MNT_PATH_LENGTH]; /*!< The mount path of MTP storage */
    int storage_count;                  /*!< The storage count on the MTP device. */
    PD_LINK_SPEED speed;                /*!< USB Link Speed. */
} MTP_INFO;

/**
 * @struct      MTP_STORAGE_INFO
 * @brief       MTP Storage Information.
 */
typedef struct _MTP_STORAGE_INFO {
    int mtp_id;                                 /*!< MTP logical ID for MTP related API. */
    int storage_id;                             /*!< MTP storage logical ID. */
    char storage_name[MTP_STORAGE_NAME_LENGTH]; /*!< MTP storage name. EX: "Internal Storage". */
    char storage_path[MTP_SYS_ID_LENGTH];       /*!< MTP storage path. EX: "/store_00010001". */
    unsigned long long total_size;              /*!< Storage total size (KBytes). */
    unsigned long long free_size;               /*!< Storage free size (KBytes). */
} MTP_STORAGE_INFO;

/**
 * @fn int MTP_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN MTP_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate mounted MTP device from the internal configuration file.
 *
 * This function is called by nas utility to obtain mounted MTP device count
 * in the internal configuration file.
 * @param[out] id_ary An identifier array to host the available port identifiers.
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback MTP device enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the mounted MTP device, if the count is larger than
 *         id_ary_count, the calling utility should allocate more space for identifier array to
 *         obtain all available port identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int MTP_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
                 IN MTP_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int MTP_Get_Info(IN int mtp_id, OUT MTP_INFO *mtp_infoP)
 * @brief Retrieve the specified MTP device information from the internal configuration file.
 *
 * This function is called by nas utility to obtain the specified MTP device information
 * from the internal configuration file.
 * @param[in] mtp_id Logical MTP device identifier.
 * @param[out] mtp_infoP a MTP_INFO structure pointer to host the MTP device information.
 * @retval 0 Success, the specified MTP device is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified MTP device in the
 *         internal configurationturn file.
 */
int MTP_Get_Info(IN int mtp_id, OUT MTP_INFO *mtp_infoP);

/**
 * @fn int MTP_Get_Storage_Info(IN int mtp_id, IN int storage_id, OUT MTP_STORAGE_INFO *mtp_storage_infoP)
 * @brief Retrieve the specified storage information of MTP device from configuration file.
 *
 * This function is called by nas utility to obtain the specified storage information of MTP device
 * from the internal configuration file.
 * @param[in] mtp_id Logical MTP device identifier.
 * @param[in] storage_id Logical storage identifier. (0 based)
 * @param[out] mtp_storage_infoP a MTP_STORAGE_INFO structure pointer to storage information.
 * @retval 0 Success, the specified storage information is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified storage in the
 *         internal configurationturn file.
 */
int MTP_Get_Storage_Info(IN int mtp_id, IN int storage_id, OUT MTP_STORAGE_INFO *mtp_storage_infoP);

/**
 * @fn int MTP_Get_Id_By_MTP_Sys_Id(IN char *mtp_sys_id, OUT int* mtp_idP)
 * @brief Retrieve the logical MTP device identifier via system dependent identifier.
 *
 * This function is called by HAL utility to obtain the assigned logical identifier from the
 * internal configuration file.
 * @param[in] mtp_sys_id MTP system dependent identifier, Ex: "usb.001.015".
 * @param[out] mtp_idP a logical port identifier that has been assigned to this MTP device.
 * @retval 0 Success, the specified MTP device identifier is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified MTP device identifier in the
 *         internal configurationturn file.
 */
int MTP_Get_Id_By_MTP_Sys_Id(IN char *mtp_sys_id, OUT int* mtp_idP);

/*@}*/    /* end of MTP Function */


/**
 *  @defgroup POE Function
 */

/**
 *  @ingroup POE Function
 */

/*@{*/    /* begin of POE Function */
#define POE_CONFIG  "/etc/poe.conf"
#define POE_SLOT_BITMAP "poe_slot_bitmap"
#define POE_PORT_BITMAP "poe_slot_%d_bitmap"

#define POE_SECTION_GLOBAL  "Global"
#define POE_SECTION_ID  "POE_ID_0x%x"

#define POE_FIELD_POEID "poe_id"
#define POE_FIELD_NAME  "name"
#define POE_FIELD_INTERFACE_NAME  "interface_name"
#define POE_FIELD_OP_INDEX  "op_index"
#define POE_FIELD_SYS_PATH_PCI_UPDSTREAM  "sys_path_pci_upstream"

#define MAX_POE_NAME_SIZE   64
#define MAX_POE_SECTION_SIZE   64
#define MAX_MAC_STRING_SIZE 20
#define MAX_POE_INFO_SCAN_SIZE   16
#define MAX_POE_INTERFACE_NAME  8
#define MAX_POE_PCI_SYS_PATH_LEN 512
// POE id = (slot id << 16) | (port_id)
typedef int POE_ID;
typedef int POE_SLOT_ID;
typedef int POE_PORT_ID;
#define POE_SLOT_ID(poe_id) ((0xffff0000 & (poe_id)) >> 16)
#define POE_PORT_ID(poe_id) (0x0000ffff & (poe_id))
#define POE_MAKE_ID(slot_id, port_id) (((slot_id) << 16) | ((0x0000ffff & (port_id))))

typedef int (*POE_DEVICE_ENUM_CALLBACK) (POE_ID dev_id, void* contextP);
typedef int (*POE_SLOT_ENUM_CALLBACK) (POE_SLOT_ID slot_id, void* contextP);
typedef int (*POE_PORT_ENUM_CALLBACK) (POE_SLOT_ID slot_id, POE_PORT_ID port_id, void* contextP);

typedef enum _POE_MODE
{
    POE_MODE_UNKNOWN = -1,
    POE_MODE_802_3_AF,
    POE_MODE_802_3_AT,
    POE_MODE_MAX,
} POE_MODE;

typedef enum _POE_PRIO
{
    POE_PRIO_UNKNOWN = -1,
    POE_PRIO_LOW,
    POE_PRIO_MEDIUM,
    POE_PRIO_HIGH,
    POE_PRIO_MAX,
} POE_PRIO;

typedef enum _POE_STATUS
{
    POE_STATUS_UNKNOWN = -1,
    POE_STATUS_SHORT,
    POE_STATUS_GOOD,
    POE_STATUS_CPD_TOO_HIGH,
    POE_STATUS_RSIG_TOO_LOW,
    POE_STATUS_RSIG_TOO_HIGH,
    POE_STATUS_OPEN_CIRCUIT,
    POE_STATUS_GOOD_NO_POWER,
    POE_STATUS_MAX,
} POE_STATUS;

typedef struct _POE_INFO
{
    char name[MAX_NAME_SIZE];
    char interface_name[MAX_POE_INTERFACE_NAME];
    POE_ID poe_id;
    int op_index;
    char sys_path_pci_upstream[MAX_POE_PCI_SYS_PATH_LEN];
} POE_INFO;

/**
 * @fn int POE_Scan()
 * @brief Scan the connected POE devices .
 *
 * This function will remove the original enclosure setting in the internal
 * configuration file and invoke the system dependent enumerate function to re-generate them.
 * @retval Return 0 
 */
int POE_Scan();

/**
 * @fn int POE_Enumerate(OUT POE_ID id_ary[], IN unsigned int id_ary_count,
 *         IN POE_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available POE device arrays in the system.
 * 
 * @param[out] id_ary An identifier array of POE devices. 
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available POE device arrays, if the count is larger than 
 *           id_ary_count, the calling utility should allocate more space for identifier array to
 *           obtain all available bridge array identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 * @retval -2 Generic failure.
 */
int POE_Enumerate(OUT POE_ID id_ary[], IN unsigned int id_ary_count,
        IN POE_DEVICE_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int POE_Slot_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
 *         IN POE_SLOT_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available POE slot arrays in the system.
 * 
 * @param[out] id_ary An identifier array of POE slots. 
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available POE slot arrays, if the count is larger than 
 *           id_ary_count, the calling utility should allocate more space for identifier array to
 *           obtain all available bridge array identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 * @retval -2 Generic failure.
 */
int POE_Slot_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
        IN POE_SLOT_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int POE_Port_Enumerate(IN int slot_id, OUT int id_ary[], IN unsigned int id_ary_count,
 *         IN POE_PORT_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available POE ports arrays for specified slot in the system.
 * 
 * @param[in] slot_id Specified the slot id identifier.
 * @param[out] id_ary An identifier array of POE ports for specified slot. 
 * @param[in] id_ary_count The entry count of the identifier array.
 * @param[in] callback enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >=0 Success, return the count of the available POE port arrays for specified slot , if the count is larger than 
 *           id_ary_count, the calling utility should allocate more space for identifier array to
 *           obtain all available bridge array identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 * @retval -2 Generic failure.
 */
int POE_Port_Enumerate(IN int slot_id, OUT int id_ary[], IN unsigned int id_ary_count,
        IN POE_PORT_ENUM_CALLBACK callback, IN void* contextP);


/**
 * int POE_Get_NIC_ID(IN POE_ID poe_id, OUT unsigned int *nic_id)
 * @brief Get NIC ID by POE device ID.
 *
 * This function will get NIC ID by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] nic_id NIC identifier.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_NIC_ID(IN POE_ID poe_id, OUT unsigned int *nic_id);

/**
 * int POE_Get_ID_by_NIC_ID(IN unsigned int  nic_id, OUT POE_ID *poe_id)
 * @brief Get POE ID by NIC ID.
 *
 * This function will get POE ID by NIC ID.
 * @param[in] nic_id NIC identifier.
 * @param[out] poe_id POE device identifier.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_ID_by_NIC_ID(IN unsigned int  nic_id, OUT POE_ID *poe_id);

/**
 * int POE_Get_Mode(IN POE_ID poe_id, OUT POE_MODE *mode)
 * @brief Get mode by POE device ID.
 *
 * This function will get mode by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] mode Mode of POE device.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Mode(IN POE_ID poe_id, OUT POE_MODE *mode);

/**
 * int POE_Set_Mode(IN POE_ID poe_id, IN POE_MODE mode)
 * @brief Get mode by POE device ID.
 *
 * This function will set mode by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[in] mode Mode of POE device.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Set_Mode(IN POE_ID poe_id, IN POE_MODE mode);


/**
 * int POE_Get_Priority(IN POE_ID poe_id, OUT POE_PRIO *priority)
 * @brief Get priority by POE device ID.
 *
 * This function will get mode by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] priority Priority of POE device.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Priority(IN POE_ID poe_id, OUT POE_PRIO *priority);

/**
 * int POE_Set_Priority(IN POE_ID poe_id, IN POE_PRIO priority)
 * @brief Set priority by POE device ID.
 *
 * This function will set mode by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] priority Priority of POE device.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Set_Priority(IN POE_ID poe_id, IN POE_PRIO priority);

/**
 * int POE_Get_Current_Power(IN POE_ID poe_id, OUT int *power)
 * @brief Get Currentent power by POE device ID.
 *
 * This function will get current power by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] power Power of POE device, unit Watt.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Current_Power(IN POE_ID poe_id, OUT int *power);

/**
 * int POE_Get_PD_Request(IN POE_ID poe_id, OUT int *pd_request)
 * @brief Get PD requested power by POE device ID.
 *
 * This function will get pd requested power by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] pd_request PD request power of POE device, unit Watt.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_PD_Request(IN POE_ID poe_id, OUT int *pd_request);

/**
 * int POE_Get_Power_Limit(IN POE_ID poe_id, OUT int *limit)
 * @brief Get Max limited power by POE device ID.
 *
 * This function will get max limited power by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[out] limit Max limited power of POE device, unit Watt.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Power_Limit(IN POE_ID poe_id, OUT int *limit);

/**
 * int POE_Set_Power_Limit(IN POE_ID poe_id, IN int limit)
 * @brief Set Max limited power by POE device ID.
 *
 * This function will set max limited power by POE device ID.
 * @param[in] poe_id POE device identifier.
 * @param[in] limit Max limited power of POE device, unit Watt.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Set_Power_Limit(IN POE_ID poe_id, IN int limit);

/**
 * int POE_Power_Enable(IN POE_ID poe_id, IN char on_off)
 * @brief Turn on/off specified POE port.
 *
 * This function will turn on/off specified POE port.
 * @param[in] poe_id POE device identifier.
 * @param[in] on_off 1 for turn on and 0 for turn off
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Power_Enable(IN POE_ID poe_id, IN char on_off);

/**
 * int POE_Get_Power_Status(IN POE_ID poe_id, OUT char *on_off)
 * @brief Query power status on or off specified POE port.
 *
 * This function will return power status of specified POE port.
 * @param[in] poe_id POE device identifier.
 * @param[out] on_off 1 for power on and 0 for power off
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Power_Status(IN POE_ID poe_id, OUT char *on_off);

/**
 * int POE_Get_Port_Status(IN POE_ID poe_id, OUT POE_STATUS *status)
 * @brief Query port status of specified POE port.
 *
 * This function will return the status of specified POE port.
 * @param[in] poe_id POE device identifier.
 * @param[out] status The poe port status
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Port_Status(IN POE_ID poe_id, OUT POE_STATUS *status);

/**
 * int POE_Get_Tempture(IN POE_SLOT_ID slot_id, OUT float *tempture)
 * @brief Query poe of specified POE slot tempture.
 *
 * This function will return the tempture of specified POE chip.
 * @param[in] slot_id POE slot identifier.
 * @param[out] tempture The poe chip tempture.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Tempture(IN POE_SLOT_ID slot_id, OUT float *tempture);
/**
 * int POE_Get_Tempture_Alarm(IN POE_SLOT_ID slot_id, OUT int *alarm)
 * @brief Query poe of specified POE tempture over alarm tempture or not.
 *
 * This function will return specified POE chip tempture over alarm tempture or not.
 * @param[in] slot_id POE slot identifier.
 * @param[out] alarm Over alarm tempture or not.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Tempture_Alarm(IN POE_SLOT_ID slot_id, OUT int *alarm);
/**
 * int POE_Get_Slot_Current_Power(IN POE_SLOT_ID slot_id, OUT int *power)
 * @brief Query poe of specified POE slot power consumption.
 *
 * This function will return the power consumption of specified POE chip.
 * @param[in] slot_id POE slot identifier.
 * @param[out] power The power consumption of poe chip.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Slot_Current_Power(IN POE_SLOT_ID slot_id, OUT int *power);
/**
 * int POE_Get_Slot_Total_Power_Supply(IN POE_SLOT_ID slot_id, OUT int *power)
 * @brief Query poe of specified POE slot total power supply.
 *
 * This function will return the total power supply  of specified POE chip.
 * @param[in] slot_id POE slot identifier.
 * @param[out] power The total power of poe chip.
 * @retval 0 Success.
 * @retval -1 Failed.
 */
int POE_Get_Slot_Total_Power_Supply(IN POE_SLOT_ID slot_id, OUT int *power);
// test used
int POE_Read_Reg(IN unsigned char addr, IN unsigned char reg, IN unsigned char *value);
int POE_Write_Reg(IN unsigned char addr, IN unsigned char reg, IN unsigned char value);
int POE_Read_Reg_PCI(IN unsigned char addr, IN unsigned char reg, IN unsigned char *value, IN char *sys_path_pci_upstream);
int POE_Write_Reg_PCI(IN unsigned char addr, IN unsigned char reg, IN unsigned char value, IN char *sys_path_pci_upstream);
/*@}*/    /* end of POE Function */

#define MAX_FC_WWN_LEN          32
#define MAX_FC_SYS_ID_LEN       32
#define MAX_FC_PORT_NUM         32
#define MAX_FC_HBA_NUM          8

typedef enum _FC_PORT_TYPE
{
    FC_PORT_TYPE_UNKNOWN = -1,
    FC_PORT_TYPE_P2P = 1,
    FC_PORT_TYPE_FABRIC,
    FC_PORT_TYPE_LOOP,
} FC_PORT_TYPE;

typedef enum _FC_PORT_STATE
{
    FC_PORT_STATE_LINKDOWN = 0,
    FC_PORT_STATE_LINKUP,
} FC_PORT_STATE;

typedef enum _FC_PORT_SPEED
{
    FC_PORTSPEED_UNKNOWN        = 0,
    FC_PORTSPEED_1GBIT          = 1,
    FC_PORTSPEED_2GBIT          = 2,
    FC_PORTSPEED_10GBIT         = 4,
    FC_PORTSPEED_4GBIT          = 8,
    FC_PORTSPEED_8GBIT          = 0x10,
    FC_PORTSPEED_16GBIT         = 0x20,
    FC_PORTSPEED_32GBIT         = 0x40,
    FC_PORTSPEED_20GBIT         = 0x80,
    FC_PORTSPEED_40GBIT         = 0x100,
    FC_PORTSPEED_50GBIT         = 0x200,
    FC_PORTSPEED_100GBIT        = 0x400,
    FC_PORTSPEED_25GBIT         = 0x800,
    FC_PORTSPEED_64GBIT         = 0x1000,
    FC_PORTSPEED_128GBIT        = 0x2000,
    FC_PORTSPEED_NOT_NEGOTIATED = (1 << 15),
} FC_PORT_SPEED;

/**
 * @struct      FC_PORT_INFO
 * @brief       FC Port Information.
 */
typedef struct _FC_PORT_INFO
{
    int hba_id;
    int port_id;
    char fc_port_sys_id[MAX_FC_SYS_ID_LEN];
    char fc_hba_sys_id[MAX_FC_SYS_ID_LEN];
    char vendor[MAX_VENDOR_LEN];
    char model[MAX_MODEL_LEN];
    char node_name[MAX_FC_WWN_LEN];
    char port_name[MAX_FC_WWN_LEN];
    FC_PORT_SPEED supported_speeds;    
#if 0   
    int fc_nport_id;
    FC_PORT_STATE port_state;
    FC_PORT_SPEED speed;
    FC_PORT_TYPE port_type;
    char fabric_name[MAX_FC_WWN_LEN];
#endif    
} FC_PORT_INFO;

/**
 * @struct      FC_HBA_INFO
 * @brief       FC HBA Information.
 */
typedef struct _FC_HBA_INFO
{
    int hba_id;
    char fc_hba_sys_id[MAX_FC_SYS_ID_LEN];
    int pcie_slot_pos;
} FC_HBA_INFO;

/**
 * @fn          typedef int (*FC_HBA_ENUM_CALLBACK) (IN int hba_id, IN void* contextP)
 * @brief       FC HBA enumeration callback function prototype.
 * @param[in]   hba_id FC HBA identifier.
 * @param[in]   contextP Callback parameter which is provided in registration.
 * @retval      0 Success, callback function is executed successfully.
 * @retval      -1 Generic failure, callback function want to abort the enumeration.
 */
typedef int (*FC_HBA_ENUM_CALLBACK) (IN int hba_id, IN void* contextP);

/**
 * @fn int FC_Scan(void)
 * @brief Scan the FC ports in the system and store them in the internal configuration file.
 *
 * This function will remove the original FC HBAs and FC ports setting in the internal
 * configuration file and invoke the system dependent enumerate function to re-generate them.
 * @retval Return the count of the available FC ports.
 */
int FC_Scan(void);

/**
 * @fn int FC_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
 *                      IN FC_HBA_ENUM_CALLBACK callback, IN void* contextP)
 * @brief Enumerate available FC HBA from the internal configuration file.
 *
 * This function is called by nas utility to obtain all available FC HBA
 * identifiers in the internal configuration file.
 * @param[out] id_ary An identifier array to host the available FC HBA identifiers.
 * @param[in] id_ary_count The entry count of the FC HBA identifier array.
 * @param[in] callback FC HBA enumeration callback function.
 * @param[in] contextP Callback function context pointer.
 * @retval >= 0 Success, return the count of the available FC HBA, if the count
 *         is larger than id_ary_count, the calling utility should allocate more
 *         space for identifier array to obtain all available FC HBA
 *         identifiers.
 * @retval -1 Abort, user callback abort the enumeration operations.
 */
int FC_Enumerate(OUT int id_ary[], IN unsigned int id_ary_count,
                  IN FC_HBA_ENUM_CALLBACK callback, IN void* contextP);

/**
 * @fn int FC_Get_HBA_Count(OUT int *hba_count)
 * @brief Retrieve the total count of FC HBAs in this system.
 *
 * This function is called by nas utility to obtain the total count of FC HBAs in this system.
 * @param[out] hba_count the total count of FC HBAs in this system.
 * @retval 0 Success, the total count of the FC HBAs in this system is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the total count of FC HBAs in this system.
 */
int FC_Get_HBA_Count(OUT int *hba_count);

/**
 * @fn int FC_Get_HBA_Bitmap(OUT int *hba_bitmapP)
 * @brief Retrieve the id bitmap of FC HBAs in this system.
 *
 * This function is called by nas to obtain the id bitmap of FC HBAs in this system.
 * @param[out] hba_bitmapP the id bitmap of FC HBAs in this system.
 * @retval 0 Success, the id bitmap of the FC HBAs in this system is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the id bitmap of FC HBAs in this system.
 */
int FC_Get_HBA_Bitmap(OUT int *hba_bitmapP);

/**
 * @fn int FC_Get_HBA_Sys_Id(IN int hba_id, OUT char *fc_hba_sys_id, IN int max_len)
 * @brief Retrieve the fc_hba_sys_id of the specified FC HBA from the internal configuration file.
 *
 * This function is called by nas utility to obtain the fc_hba_sys_id of the specified FC HBA from the internal configuration file.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[out] fc_hba_sys_id the fc_hba_sys_id of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the fc_hba_sys_id.
 * @retval 0 Success, the fc_hba_sys_id of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the fc_hba_sys_id of the specified FC HBA.
 */
int FC_Get_HBA_Sys_Id(IN int hba_id, OUT char *fc_hba_sys_id, IN int max_len);

/**
 * @fn int FC_Get_HBA_PCIe_Slot_Pos(IN int hba_id, OUT int *pcie_slot_pos)
 * @brief Retrieve the PCIe slot position of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the PCIe slot position of the specified FC HBA.
 * @param[in] hba_id the FC HBA identifier of the specified FC HBA.
 * @param[out] pcie_slot_pos the PCIe slot position of the specified FC HBA.
 * @retval 0 Success, the PCIe slot position of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the PCIe slot position of the specified FC HBA.
 */
int FC_Get_HBA_PCIe_Slot_Pos(IN int hba_id, OUT int *pcie_slot_pos);

/**
 * @fn int FC_Get_HBA_Port_Count(IN int hba_id, OUT int *port_count)
 * @brief Retrieve the total port count of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the total port count of the specified FC HBA.
 * @param[in] hba_id the FC HBA identifier of the specified FC HBA.
 * @param[out] port_count the total port count of the specified FC HBA.
 * @retval 0 Success, the total port count of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the total port count of the specified FC HBA.
 */
int FC_Get_HBA_Port_Count(IN int hba_id, OUT int *port_count);

/**
 * @fn int FC_Get_HBA_Port_Bitmap(IN int hba_id, OUT int *port_bitmapP)
 * @brief Retrieve the FC port id bitmap of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the FC port id bitmap of the specified FC HBA.
 * @param[in] hba_id the FC HBA identifier of the specified FC HBA.
 * @param[out] port_bitmapP the FC port id bitmap of the specified FC HBA.
 * @retval 0 Success, the FC port id bitmap of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the FC port id bitmap of the specified FC HBA.
 */
int FC_Get_HBA_Port_Bitmap(IN int hba_id, OUT int *port_bitmapP);

/**
 * @fn int FC_Get_Port_Sys_Id(IN int hba_id, IN int port_id, OUT char *fc_port_sys_id, IN int max_len)
 * @brief Retrieve the fc_port_sys_id of the specified FC port of the specified FC HBA from the internal configuration file.
 *
 * This function is called by nas utility to obtain the fc_port_sys_id of the specified FC port of the specified FC HBA from the internal configuration file.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] fc_port_sys_id the fc_port_sys_id of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the fc_port_sys_id.
 * @retval 0 Success, the fc_port_sys_id of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the fc_port_sys_id of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Sys_Id(IN int hba_id, IN int port_id, OUT char *fc_port_sys_id, IN int max_len);

/**
 * @fn int FC_Get_Port_Vendor_Name(IN int hba_id, IN int port_id, OUT char *vendor_name, IN int max_len)
 * @brief Retrieve the vendor name of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the vendor name of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] vendor_name the vendor name of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the vendor name.
 * @retval 0 Success, the vendor name of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the vendor name of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Vendor_Name(IN int hba_id, IN int port_id, OUT char *vendor_name, IN int max_len);

/**
 * @fn int FC_Get_Port_Model_Name(IN int hba_id, IN int port_id, OUT char *model_name, IN int max_len)
 * @brief Retrieve the model name of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the model name of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] model_name the model name of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the model name.
 * @retval 0 Success, the model name of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the model name of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Model_Name(IN int hba_id, IN int port_id, OUT char *model_name, IN int max_len);

/**
 * @fn int FC_Get_Port_WWNN(IN int hba_id, IN int port_id, OUT char *node_name, IN int max_len)
 * @brief Retrieve the WWNN of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the WWNN of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] node_name the WWNN of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the WWNN.
 * @retval 0 Success, the WWNN of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the WWNN of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_WWNN(IN int hba_id, IN int port_id, OUT char *node_name, IN int max_len);

/**
 * @fn int FC_Get_Port_WWPN(IN int hba_id, IN int port_id, OUT char *port_name, IN int max_len)
 * @brief Retrieve the WWPN of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the WWPN of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] port_name the WWPN of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the WWPN.
 * @retval 0 Success, the WWPN of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the WWPN of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_WWPN(IN int hba_id, IN int port_id, OUT char *port_name, IN int max_len);

/**
 * @fn int FC_Get_Port_Supported_Speeds(IN int hba_id, IN int port_id, OUT FC_PORT_SPEED *supported_speeds)
 * @brief Retrieve the supported speeds of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the supported speeds of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] supported_speeds the supported speeds of the specified FC port of the specified FC HBA.
 * @retval 0 Success, the supported speeds of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the supported speeds of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Supported_Speeds(IN int hba_id, IN int port_id, OUT FC_PORT_SPEED *supported_speeds);

/**
 * @fn int FC_Get_Port_NPortId(IN int hba_id, IN int port_id, OUT int *fc_nport_idP)
 * @brief Retrieve the current N_Port_ID of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the current N_Port_ID of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] fc_nport_idP the current N_Port_ID of the specified FC port of the specified FC HBA.
 * @retval 0 Success, the current N_Port_ID of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the current N_Port_ID of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_NPortId(IN int hba_id, IN int port_id, OUT int *fc_nport_idP);

/**
 * @fn int FC_Get_Port_State(IN int hba_id, IN int port_id, OUT FC_PORT_STATE *port_stateP)
 * @brief Retrieve the current port state of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the current port state of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] port_stateP the current port state of the specified FC port of the specified FC HBA. e.g. LinkUp, LinkDown.
 * @retval 0 Success, the current port state of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the current port state of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_State(IN int hba_id, IN int port_id, OUT FC_PORT_STATE *port_stateP);

/**
 * @fn int FC_Get_Port_Speed(IN int hba_id, IN int port_id, OUT FC_PORT_SPEED *port_speedP)
 * @brief Retrieve the current port speed of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the current port speed of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] port_speedP the current port speed of the specified FC port of the specified FC HBA.
 * @retval 0 Success, the current port speed of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the current port speed of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Speed(IN int hba_id, IN int port_id, OUT FC_PORT_SPEED *port_speedP);

/**
 * @fn int FC_Get_Port_Type(IN int hba_id, IN int port_id, OUT FC_PORT_TYPE *port_typeP)
 * @brief Retrieve the current port type of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the current port type of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] port_typeP the current port type of the specified FC port of the specified FC HBA. e.g. Fabric, P2P.
 * @retval 0 Success, the current port type of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the current port type of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Type(IN int hba_id, IN int port_id, OUT FC_PORT_TYPE *port_typeP);

/**
 * @fn int FC_Get_Port_Fabric_WWPN(IN int hba_id, IN int port_id, OUT char *fabric_port_nameP, IN int max_len)
 * @brief Retrieve the fabric WWPN of the specified FC port of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the fabric WWPN of the specified FC port of the specified FC HBA.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] fabric_port_nameP the fabric WWPN of the specified FC port of the specified FC HBA.
 * @param[in] max_len the maximum buffer length to store the fabric WWPN.
 * @retval 0 Success, the fabric WWPN of the specified FC port of the specified FC HBA is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the fabric WWPN of the specified FC port of the specified FC HBA.
 */
int FC_Get_Port_Fabric_WWPN(IN int hba_id, IN int port_id, OUT char *fabric_port_nameP, IN int max_len);

/**
 * @fn int FC_Get_Port_Info(IN int hba_id, IN int port_id, OUT FC_PORT_INFO *fcport_infoP)
 * @brief Retrieve the specified FC port information of the specified FC HBA.
 *
 * This function is called by nas utility to obtain the specified FC port object information.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[in] port_id the FC port identifier of the specified FC port.
 * @param[out] fcport_infoP a FC_PORT_INFO structure pointer to host the FC port object information.
 * @retval 0 Success, the specified FC port object is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified FC port object.
 */
int FC_Get_Port_Info(IN int hba_id, IN int port_id, OUT FC_PORT_INFO *fcport_infoP);

/**
 * @fn int FC_Get_HBA_Info(IN int hba_id, FC_HBA_INFO *fchba_infoP)
 * @brief Retrieve the specified FC HBA information.
 *
 * This function is called by nas utility to obtain the specified FC HBA object information.
 * @param[in] hba_id the HBA identifier of the specified FC HBA.
 * @param[out] fchba_infoP a FC_HBA_INFO structure pointer to host the FC HBA object information.
 * @retval 0 Success, the specified FC HBA object is retrieved completely.
 * @retval -1 Generic failure, Fail to retrieve the specified FC HBA object.
 */
int FC_Get_HBA_Info(IN int hba_id, OUT FC_HBA_INFO *fchba_infoP);

/**
 * @fn int SE_UJBOD_Create_PD_Monitor(IN char *enc_sys_id)
 * @brief Some UJBOD need a process to detect pd exist or not
 *
 * This function create a process to polling device status.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @retval 0 The specified create process successfully.
 * @retval -1 The specified create process fail.
 */
int SE_UJBOD_Create_PD_Monitor(IN char *enc_sys_id);

/**
 * @fn int SE_UJBOD_Destroy_PD_Monitor(IN char *enc_sys_id)
 * @brief Remove detect pd exist process for UJBOD.
 *
 * This function destroy the UJBOD pd monitor process.
 * @param[in] enc_sys_id Enclosure system dependent identifier.
 * @retval 0 The specified destroy process successfully.
 * @retval -1 The specified destroy process fail.
 */
int SE_UJBOD_Destroy_PD_Monitor(IN char *enc_sys_id);

/**
 * @fn int PD_Inc_ATA_ErrCnt(char *sn, int tnum, int id)
 * @brief Increase ATA Error count. 
 *
 * This function is called by nas utility to increase the error count for drive.
 * @param[in] *sn is the serial number of the disk.
 * @param[in] tnum is the template number defined by ULINK spec.
 * @param[in] id is the cell if defined by ULINK spec.
 * @retval 0 Success, the value is increased sucessful.
 * @retval -1 Generic failure, Fail to increase the value.
 */
int PD_Inc_ATA_ErrCnt(char *sn, int tnum, int id);

/**
 * @fn int PD_Set_ATA_IOTest_Val(char *sn, int tnum, int id)
 * @brief Set IOTest Value for ATA drive.
 *
 * This function is called by nas utility to set the value of iotest result.
 * @param[in] *sn is the serial number of the disk.
 * @param[in] tnum is the template number defined by ULINK spec.
 * @param[in] id is the cell if defined by ULINK spec.
 * @retval 0 Success, the value is set successful.
 * @retval -1 Generic failure, Fail to set the value.
 */
int PD_Set_ATA_IOTest_Val(char *sn, int tnum, int id, int val);

/**
 * @fn int PD_Set_ATA_IO_Latency(PD_INFO *pd_info)
 * @brief Set io latency for ata disk.
 *
 * This function is called by nas utility to set the value for io latency.
 * @param[in] *pd_info is the physical disk info.
 * @retval 0 Success, the value is set successful.
 * @retval -1 Generic failure, Fail to set the value.
 */
int PD_Set_ATA_IO_Latency(PD_INFO *pd_info);

/**
 * @fn int PD_Get_Disk_Statistic_Data(IN int enc_id, IN int port_id, IN int tnum, OUT char *buf)
 * @brief Get the disk statistic data by enc_id, port_id and template number.
 *
 * This function will get the disk statistic data by enc_id, port_id and template number.
 *
 * @param[in] enc_id Enclosure id
 * @param[in] port_id Disk Port id
 * @param[in] tnum TEMPLATE NUMBER
 * @param[out] *buf Buffer for the disk statistic data.
 *
 * @retval 0 Success.
 * @retval -1 Generic failure.
 */
int PD_Get_Disk_Statistic_Data(IN int enc_id, IN int port_id, IN int tnum, OUT char *buf);
/*@}*/  /* end of upper layer module */

/**
 * @todo To do test.
 */

//------------------------------------------------
//TL/TR Jbod 
typedef enum _SES_CMD_TYPE
{	
	GET_SYS_TEMPERATURE,	
	GET_FAN_SWITCH,
	GET_FAN_SPEED,	
	GET_LCM_BUTTON,
	SET_FAN_SPEED,	
	SET_FAN_ERROR_LED,
	SET_STATUS_LED,			
	SET_THUNDERBOLT_LED,	
	SET_HDD_LED,			
	SET_ALL_HDD_LED,
	SET_LINK_LED,
	SET_BUZZER,
	SET_LCM_DISPLAY,
	GET_VPD,
	SET_VPD,
	RESET_RESERVED_BIT,
	DOWNLOAD_EC,
	GET_EC_VER,
	SET_FAN_MODE
} SES_CMD_TYPE;

typedef enum _SES_PAGE_CODE
{	
	CONFIGURE_DIAG_PAGE 		= 0x01,
	ENCLOSURE_DIAG_PAGE 		= 0x02,
	STRING_DIAG_PAGE 			= 0x04,
	DOWNLOAD_MICROCODE_DIAG_PAGE= 0x0E,
	VENDOR_SPEC_SES_DIAG_PAGE1 	= 0x10, 
	VENDOR_SPEC_SES_DIAG_PAGE2 	= 0x11,
	VENDOR_SPEC_SES_DIAG_PAGE3 	= 0x12,
	VENDOR_SPEC_SES_DIAG_PAGE4 	= 0x13
} SES_PAGE_CODE;

typedef enum _SES_PAGE_ELEMENT_TYPE
{	
	ELEMENT_COOLING 				= 0x03,
	ELEMENT_TEMPERATURE 			= 0x04,
	ELEMENT_ALARM 					= 0x06,
	ELEMENT_ENCLOSURE_LED 			= 0x0E,
	ELEMENT_SCSI_PORT_TRANSCEIVER 	= 0x0F,
	ELEMENT_HDD_LED 				= 0x17
} SES_PAGE_ELEMENT_TYPE;

/*
typedef enum _SES_PAGE_OFFSET
{	
	OFFSET_SERIAL_NUM = 0x00,		// offset = 0
	OFFSET_MODEL_NAME = 0x10		// offset = 16
} SES_PAGE_OFFSET;
*/

#define HWTEST_DBG_FILE		"/tmp/dbg_halhwtest"

//TL/TR Jbod
//------------------------------------------------
int SG_SES_COMMAND(IN SES_CMD_TYPE ses_cmd_type,IN char * enc_sys_id,IN int index,OUT char * pVal);
int SG_VENDOR_SES_COMMAND(IN SES_CMD_TYPE ses_cmd_type, IN char *enc_sys_id, IN VPD_LOC vpd_pos, IN unsigned int vpd_offset, IN int vpd_length, unsigned char* buf);
int TL_GET_PORT_NUM(char *sd_dev, unsigned char *addr, unsigned char *sep_status);

#ifdef __cplusplus
}
#endif      /* __cplusplus */

#endif
