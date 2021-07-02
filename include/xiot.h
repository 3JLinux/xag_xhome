#ifndef _XIOT_H_
#define _XIOT_H_

/*  */
typedef void* xiot_handler;

/*  */
typedef struct {
    char id[64];        // Device ID
    int type;           // Device Type
    int model;          // Device Model
    char version[16];   // Device Software Version
    struct {
        char id[64];    // Gateway ID
        int type;       // Gateway Type
        int model;      // Gateway Model
        char iccid[32]; // Gateway ICCID (Get from SIM Card)
    } gateway;
} xiot_device_t;

/*  */
typedef struct {
    int battery;        // Device Battery Level (0~100)
    int charging;       // Device Charging Flag (0:No Charging 1:Charging)
    int rssi;           // Device RSSI (Signal Strength dBm)
} xiot_device_status_t;

/*  */
typedef struct {
    double latitude;    // Device Latitude
    double longitude;   // Device Longitude
    double altitude;    // Device Altitude (cm)
    double precision;   // Device Precision (cm)
} xiot_location_t;

/*  */
typedef struct {
    int mnc;    // Mobile Network Code
    int mcc;    // Mobile Country Code
    int ci;     // Cell ID
    int lac;    // Location Area Code
} xiot_cell_t;

/*  */
typedef struct {
    int id;             // Unique ID
    int src;            // Source Address
    int dest;           // Destination Address
    char subject[128];  // Message Subject
    int size;           // Message Body Size
    char body[0];       // Message Body Buffer
} xiot_message_t;

typedef struct {
    int id;                     // Unique ID
    char from[64];              // Source Address
    char to[256];               // devid
    char subject[128];          // Message Subject
    int size;                   // Message Body Size
    char body[0];               // Message Body Buffer
} xiot_message_dev_t;

/*  */
typedef struct {
    xiot_location_t *location;  // Data Collection Location
    unsigned int create_at;     // Data Collection Time (UTC)
    int type;                   // Data Collection Type
    int size;                   // Data Size
    char data[0];               // Data Buffer
} xiot_datapoint_t;

typedef struct {
    xiot_location_t location;  // Data Collection Location
    unsigned int create_at;     // Data Collection Time (UTC)
    int type;                   // Data Collection Type
    int size;                   // Data Size
    char data[0];               // Data Buffer
} xiot_datapoint_clp_t;
/*  */
typedef struct {
    xiot_location_t *location;  // Data Collection Location
    unsigned int create_at;     // Data Collection Time (UTC)
    int type;                   // Data Collection Type
    int size;                   // Data Size
    float data[256];            // Data Array
} xiot_datapoint_value_t;

/*  */
typedef struct {
    xiot_location_t *location;  // Data Collection Location
    unsigned int create_at;     // Data Collection Time (UTC)
    int type;                   // Data Collection Type
    int width;                  // Image Width
    int height;                 // Image Height
    char content_type[64];      // Image Content Type. Sample 'image/jpg'
    char local_path[256];       // Image Local Path
} xiot_datapoint_image_t;

/*  */
typedef struct {
    int type;           // Alarm Type (0:Create)
    char name[64];      // Alarm Name
    char detail[512];   // Alarm Detail
} xiot_alarm_t;

/*  */
typedef struct {
    // Message Event Callback (Receive from Server)
    int (*message)(xiot_message_t *m);
    // Device Start Software Upgrade Event Callback
    int (*start_upgrade)(const char *path, const char *version);
    // Device Complete Software Upgrade Event Callback
    int (*complete_upgrade)(const char *path, const char *version);
    // Network Exeception Event Callback
    int (*exception)(int code);
} xiot_event_t;

/**
 @brief Create object resources
 @param [in]    encrypt
    NULL    No Encryption
    "AES"   AES Encryption
 @return Object Handler
 **/
xiot_handler xiot_create(const char *encrypt);

/**
 @brief Destroy object resources
 @param
 @return
 **/
void xiot_destroy(xiot_handler handler);

/**
 @brief Initialize the device object
 @param [in]    device_id  Device ID
 @param [in]    type       Device Type
 @param [in]    model      Device Model
 @param [in]    version    Device Software Version
 @param [in]    imei       GSM Moudle IEMI Code
 @param [in]    iccid      SIM Card ICCID Code
 @param [out]   device     Device Object Handler
 @return
 **/
void xiot_device_init(const char *device_id, int type, int model,
                      const char *version, const char *imei, const char *iccid, xiot_device_t *device);
/*  */

/**
 @brief Connect to the server
 @param [in]    handler
 @param [in]    host_uri    Server URI. When this parameter is set to NULL, the default URI will be used
 @param [in]    device      Device Object Handler
 @return    0   success
 **/
int xiot_connect(xiot_handler handler, const char *host_uri, xiot_device_t *device);

/**
 @brief Disconnect from the server.
 @param [in]    handler
 @return    0   success
 **/
int xiot_disconnect(xiot_handler handler);

/**
 @brief Check connection is valid.
 @param [in]    handler
 @return    0   success
 **/
int xiot_connected(xiot_handler handler);

/**
 @brief Set device status information.
 @param [in]    handler
 @param [in]    status
 @return    0   success
 **/
int xiot_set_status(xiot_handler handler, xiot_device_status_t *status);

/**
 @brief Set device location.
 @param [in]    handler
 @param [in]    location    Device Location. When this parameter is set to NULL, the location
                            informatiom is cleared.
 @return    0   success
 **/
int xiot_set_location(xiot_handler handler, xiot_location_t *location);

/**
 @brief Set device base station cell information.
 @param [in]    handler
 @param [in]    cell    Base Station Cell. (MNC, MCC, CI, LA)
 @return    0   success
 **/
int xiot_set_cell(xiot_handler handler, xiot_cell_t *cell);

/**
 @brief Set device base station network information.
 @param [in]    handler
 @param [in]    mode    Base Station Network Mode. (GSM, LET, WCDMA)
 @return    0   success
 **/
int xiot_set_network_mode(xiot_handler handler, const char *mode);

/**
 @brief Set device software upgrade dowload path. When the device is set to upgrade configuration,
        download the software files from the server to this path. Receive upgrade event after the
        download is complete.
 @param [in]    handler
 @param [in]    path
 @return    0   success
 **/
int xiot_set_software_download_path(xiot_handler handler, const char *path);

/**
 @brief Get server uri.
 @param [in]    handler
 @param [out]   uri buffer
 @param [in]    uri buffer size
 @note XIot devices use.
 @return    0   success
 **/
int xiot_get_server_uri(xiot_handler handler, char *uri, int size);

/**
 @brief Enable device internal event.
 @param [in]    handler
 @param [in]    enable
    0   Disable
    1   Enable
 @note XIot devices use.
 @return    0   success
 **/
int xiot_enable_internal_event(xiot_handler handler, int enable);

/**
 @brief Get device business code.
 @param [in]    handler
 @note XIot devices use.
 @return    0   success
 **/
int xiot_get_business_code(xiot_handler handler);

/**
 @brief Get device custon configuration, output string.
 @param [in]    handler
 @param [in]    key     Parameter Key
 @param [out]   value   Parameter Value
 @return    0   success
 **/
int xiot_get_param(xiot_handler handler, const char *key, char *value);

/**
 @brief Get device custon configuration, output int32.
 @param [in]    handler
 @param [in]    key     Parameter Key
 @param [out]   value   Parameter Value
 @return    0   success
 **/
int xiot_get_param_int32(xiot_handler handler, const char *key, int *value);

/**
 @brief Get device custon configuration, output double.
 @param [in]    handler
 @param [in]    key     Parameter Key
 @param [out]   value   Parameter Value
 @return    0   success
 **/
int xiot_get_param_double(xiot_handler handler, const char *key, double *value);

/**
 @brief Get device debug configuration.
 @param [in]    handler
 @param [in]    key     Parameter Key
 @param [out]   value   Parameter Value
 @return    0   success
 **/
int xiot_get_debug_param(xiot_handler handler, const char *key, char *value);

/**
 @brief Get device debug configuration.
 @param [in]    handler
 @param [in]    key     Parameter Key
 @param [out]   value   Parameter Value
 @return    0   success
 **/
int xiot_get_debug_param_int32(xiot_handler handler, const char *key, int *value);

/**
 @brief Post message to server.
 @param [in]    handler
 @param [in]    message
 @return    0   success
 **/
int xiot_post_message(xiot_handler handler, xiot_message_t *m);

/**
 @brief Post datapoint to server.
 @param [in]    handler
 @param [in]    point
 @return    0   success
 **/
int xiot_post_datapoint(xiot_handler handler, xiot_datapoint_t *point);

/**
 @brief Post datapoint to server.
 @param [in]    handler
 @param [in]    point
 @return    0   success
 **/
int xiot_post_datapoint_value(xiot_handler handler, xiot_datapoint_value_t *point);

/**
 @brief Post datapoint to server.
 @param [in]    handler
 @param [in]    point
 @return    0   success
 **/
int xiot_post_datapoint_image(xiot_handler handler, xiot_datapoint_image_t *point);

/**
 @brief Post alarm to server.
 @param [in]    handler
 @param [in]    alarm
 @return    0   success
 **/
int xiot_post_alarm(xiot_handler handler, xiot_alarm_t *alarm);

/**
 @brief Listening server event.
 @param [in]    handler
 @param [in]    event
 @return    0   success
 **/
int xiot_listen_event(xiot_handler handler, xiot_event_t *event);

/**
 @brief Upload file to ALi OSS Server.
 @param [in]    handler
 @param [in]    local   Local File Path.
 @param [in]    remote  Remote File Path.
 @param [in]    type    Content Type. Sample 'image/jpg' 'text/plain'.
 @note  XIot devices use.
 @return    0   success
 **/
int xiot_upload_ali_cloud(xiot_handler handler, const char *local, const char *remote, const char* type);

/**
 @brief Sync Server information.
 @param [in]    handler
 @return    0   success
 **/
int xiot_refresh(xiot_handler handler);

/*  */
int xiot_data_save(xiot_datapoint_t *point, const char *file);
int xiot_data_upload(xiot_handler handler, const char *file);

/*  */
int xiot_search_host(char **list, char *target);

/*  */
int xiot_shell_start();

#endif  // _XIOT_H_
