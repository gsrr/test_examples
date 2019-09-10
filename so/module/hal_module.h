#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_BUF_LEN 256
#define MAX_CMD_LEN 512
#define MAX_PATH_LEN 256


/**
 * @struct      ENCLOSURE_INFO
 * @brief       Storage Enclosure Information.
 */
typedef struct _ENCLOSURE_INFO {
    int enc_id;                         /*!< 0 based logical index generated automatically by HAL uppper layer. */
    int enc_stack_id;           /*!< The enclosure stack position related to the root enclosure. */
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
    int conn_status[MAX_EXT_CONNECTOR_NUM]; /*!< External connector status, for SAS JBOD only .*/
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
