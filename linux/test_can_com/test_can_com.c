#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

/*
ip link set can0 type cantq 125 prop-seg 6phase-seg1 7 phase-seg2 2 sjw 1

ip link set can0 type can bitrate 125000

ip -details link show can0

ifconfig can0 up

ifconfig can0 down

ip -details -statistics link show can0

*/

#define APP_NAME "[can_com] "

#define UTILS_LOG(...)                                              \
        do {                                                        \
            printf(APP_NAME);                                       \
            printf(__VA_ARGS__);                                    \
        } while(0)
#define UTILS_ERR(...)                                              \
        do {                                                        \
            UTILS_LOG(__VA_ARGS__);                                 \
            fprintf(stderr, APP_NAME);                              \
            fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",\
                    __LINE__, __FILE__, errno, strerror(errno));    \
        } while(0)

static int can_recv_test()
{
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[1];
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    rfilter[0].can_id = 0x11;
    rfilter[0].can_mask = CAN_SFF_MASK;

    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    while(1) {
        nbytes = read(s, &frame, sizeof(frame));
        if(nbytes > 0)
        {
            printf("ID=0x%X DLC=%d data[0]=0x%X\n", frame.can_id,
                frame.can_dlc, frame.data[0]);
        }
    }
    close(s);
    return 0;
}

int main()
{
    int ret = 0;
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame[2] = {{0}};
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (s < 0) {
        ret = -1;
        UTILS_ERR("open can socket failed\n");
        goto finish;
    }
    strcpy(ifr.ifr_name, "can0" );
    ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        UTILS_ERR("ioctl failed\n");;
        goto fd_finish;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        UTILS_ERR("open bind failed\n");
        goto fd_finish;
    }

    ret = setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    if (ret < 0) {
        UTILS_ERR("setsockopt failed\n");
        goto fd_finish;
    }

    frame[0].can_id = 0x11;
    frame[0].can_dlc = 1;
    frame[0].data[0] = 'Y';
    frame[0].can_id = 0x22;
    frame[0].can_dlc = 1;
    frame[0].data[0] = 'N';

    while (1) {
        nbytes = write(s, &frame[0], sizeof(frame[0]));
        if(nbytes != sizeof(frame[0])) {
            printf("Send Error frame[0]\n!");
            //break;
        }
        sleep(1);
        nbytes = write(s, &frame[1], sizeof(frame[1]));
        if(nbytes != sizeof(frame[0])) {
            printf("Send Error frame[1]\n!");
            //break;
        }
        sleep(1);
    }
fd_finish:
    close(s);
finish:
    return ret;
}