#ifndef _wrap_sdmmc_h
#define _wrap_sdmmc_h

#include <stdint.h>

typedef uint32_t sdmmc_host_t;
typedef uint32_t sdmmc_cid_t;
typedef uint32_t sdmmc_response_t;
typedef uint32_t sdmmc_csd_t;
typedef uint32_t sdmmc_scr_t;
typedef uint32_t sdmmc_ext_csd_t;

/**
 * SD/MMC card information structure
 */
typedef struct {
    sdmmc_host_t host;          /*!< Host with which the card is associated */
    uint32_t ocr;               /*!< OCR (Operation Conditions Register) value */
    union {
        sdmmc_cid_t cid;            /*!< decoded CID (Card IDentification) register value */
        sdmmc_response_t raw_cid;   /*!< raw CID of MMC card to be decoded
                                         after the CSD is fetched in the data transfer mode*/
    };
    sdmmc_csd_t csd;            /*!< decoded CSD (Card-Specific Data) register value */
    sdmmc_scr_t scr;            /*!< decoded SCR (SD card Configuration Register) value */
    sdmmc_ext_csd_t ext_csd;    /*!< decoded EXT_CSD (Extended Card Specific Data) register value */
    uint16_t rca;               /*!< RCA (Relative Card Address) */
    uint16_t max_freq_khz;      /*!< Maximum frequency, in kHz, supported by the card */
    uint32_t is_mem : 1;        /*!< Bit indicates if the card is a memory card */
    uint32_t is_sdio : 1;       /*!< Bit indicates if the card is an IO card */
    uint32_t is_mmc : 1;        /*!< Bit indicates if the card is MMC */
    uint32_t num_io_functions : 3;  /*!< If is_sdio is 1, contains the number of IO functions on the card */
    uint32_t log_bus_width : 2; /*!< log2(bus width supported by card) */
    uint32_t is_ddr : 1;        /*!< Card supports DDR mode */
    uint32_t reserved : 23;     /*!< Reserved for future expansion */
} sdmmc_card_t;

#endif