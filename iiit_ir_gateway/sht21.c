#include "sht21.h"
#include "appconfig.h"

void SHT21GetRHInt(uint16_t *rh)
{
	esp_err_t ret;
	uint8_t buff[3] = {};

	/* Send RH wait CMD */
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_st    art(cmd);
	i2c_master_write_byte(cmd, (ADDR_SHT21 << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, SHT21_CMD_RH_WAIT, true);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(BOARD_I2C, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		return;
	}

	/* Read RH Data + Chksum Bytes */
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ADDR_SHT21 << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &buff[0], I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &buff[1], I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &buff[2], I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(BOARD_I2C, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);
    
    *rh = (((uint16_t)buff[0])<<8) | buff[1];
}

void SHT21GetTempInt(uint16_t *temp)
{
	esp_err_t ret;
	uint8_t buff[3] = {};

	/* Send TEMP wait CMD */
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ADDR_SHT21 << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, SHT21_CMD_TEMP_WAIT, true);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(BOARD_I2C, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK) {
		return;
	}

	/* Read TEMP Data + Chksum Bytes */
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (ADDR_SHT21 << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &buff[0], I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &buff[1], I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &buff[2], I2C_MASTER_NACK);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(BOARD_I2C, cmd, pdMS_TO_TICKS(1000));
	i2c_cmd_link_delete(cmd);

    *temp = (((uint16_t)buff[0])<<8) | buff[1];
}

void SHT21GetRHFlt(float *rh)
{
    uint16_t rhInt;
    SHT21GetRHInt(&rhInt);
    rhInt = rhInt &0xFFFC;
    *rh = ((0.0019073486328125  * rhInt) - 6); 
}

void SHT21GetTempFlt(float *temp)
{
    uint16_t tempInt;
    SHT21GetTempInt(&tempInt);
    tempInt = tempInt &0xFFFC;
    *temp = ((tempInt *  0.0026812744140625) - 46.85);
}

float SHT21RhConvIntToFlt(uint16_t rhInt)
{
	rhInt = rhInt &0xFFFC;
    return ((0.0019073486328125  * rhInt) - 6);
}

float SHT21TempConvIntToFlt(uint16_t tempInt)
{
    tempInt = tempInt &0xFFFC;
    return ((tempInt *  0.0026812744140625) - 46.85);
}
