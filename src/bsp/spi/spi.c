/*
*
*   file: spi.c
*   updata: 2024-12-05
*
*/

#include "bsp/spi/spi.h"

static int _spi_dev_fd;
static int _spi_initialized = 0;       
pthread_mutex_t spi_mutex = PTHREAD_MUTEX_INITIALIZER;

int spi_init(const char *spi_dev, SPI_MODE mode, SPI_SPEED speed)
{
    ssize_t ret; 
    SPI_MODE spi_mode;
    char spi_bits;
    SPI_SPEED spi_speed;

    if (spi_dev == NULL)
        return -EINVAL;

    _spi_dev_fd = open(spi_dev, O_RDWR);
	if (_spi_dev_fd < 0) {
		perror("open");
        return errno;
	}

    /* mode */
    spi_mode = mode;
    ret = ioctl(_spi_dev_fd, SPI_IOC_WR_MODE, &spi_mode);                
    if (ret < 0) {
		perror("SPI_IOC_WR_MODE");
        return errno;
	}

    /* bits per word */
    spi_bits = 8;
    ret = ioctl(_spi_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);    
    if (ret < 0) {
		perror("SPI_IOC_WR_BITS_PER_WORD");
        return errno;
	}

    /* speed */
    spi_speed = (uint32_t)speed;
    ret = ioctl(_spi_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);        
    if (ret < 0) {
		perror("SPI_IOC_WR_MAX_SPEED_HZ");
        return errno;
	}

    fprintf(stdout, " [SPI INIT] : SPI MODE  = %d\n", mode);
    fprintf(stdout, " [SPI INIT] : SPI BITs  = %d\n", spi_bits);
    fprintf(stdout, " [SPI INIT] : SPI SPEED = %d\n", spi_speed);

    _spi_initialized = 1;
    return 0;
}

static int _spi_write_then_read(unsigned char *send_buf, unsigned int send_buf_len, unsigned char *recv_buf, unsigned int recv_buf_len)
{
    struct spi_ioc_transfer	xfer[2];
	int status;

    if (_spi_initialized == 0) 
    {
        fprintf(stdout, "spidev not initialized!\n");
        return -EINVAL;
    }

    if(send_buf == NULL || recv_buf == NULL)
        return -EINVAL;

    if(send_buf_len < 1 || recv_buf_len < 1)
        return -EINVAL;

    memset(xfer, 0, sizeof(xfer));

	xfer[0].tx_buf = (unsigned long)send_buf;
	xfer[0].len = send_buf_len;

	xfer[1].rx_buf = (unsigned long)recv_buf;
	xfer[1].len = recv_buf_len;

    pthread_mutex_lock(&spi_mutex);
	status = ioctl(_spi_dev_fd, SPI_IOC_MESSAGE(2), xfer);
    pthread_mutex_unlock(&spi_mutex);

	if (status < 0) 
    {
		perror("SPI_IOC_MESSAGE");
        return errno;
	}

    return 0;
}

static int _spi_write_byte_data(unsigned char data)
{
    unsigned char buff[1] = {data};
    ssize_t ret = 0;

    if(_spi_initialized == 0)
    {
        fprintf(stdout, "spidev not initialized!\n");
        return -EINVAL;
    }

    pthread_mutex_lock(&spi_mutex);
    ret = write(_spi_dev_fd, &buff, 1);
    pthread_mutex_lock(&spi_mutex);

    if (ret < 0) 
    {
        perror("write");
        return errno;
    }

    return 0;
}

static int _spi_write_nbyte_data(unsigned char *send_buf, unsigned int send_buf_len)
{
    struct spi_ioc_transfer	xfer[2];
    unsigned char recv_buf[send_buf_len];
	int status;

    if(_spi_initialized == 0)
    {
        fprintf(stdout, "spidev not initialized!\n");
        return -EINVAL;
    }

    if(send_buf == NULL || send_buf_len < 1)
        return -EINVAL;

    memset(xfer, 0, sizeof(xfer));
    memset(recv_buf, 0, sizeof(send_buf_len));

	xfer[0].tx_buf = (unsigned long)send_buf;
    xfer[0].rx_buf = (unsigned long)recv_buf;
	xfer[0].len = send_buf_len;

    pthread_mutex_lock(&spi_mutex);
	status = ioctl(_spi_dev_fd, SPI_IOC_MESSAGE(1), xfer);
    pthread_mutex_unlock(&spi_mutex);

	if (status < 0) 
    {
		perror("SPI_IOC_MESSAGE");
		return errno;
	}

    return 0;
}

void spi_exit()
{
    if(_spi_dev_fd >= 0)
        close(_spi_dev_fd);
    
    _spi_initialized = 0;
}

static spi_operations_t _spi_ops = {
    .spi_write_then_read = _spi_write_then_read,
    .spi_write_byte_data = _spi_write_byte_data,
    .spi_write_nbyte_data = _spi_write_nbyte_data,
};

spi_operations_t *get_spi_ops()
{
    return &_spi_ops;
};
