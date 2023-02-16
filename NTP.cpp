#include <iostream>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


#define NTP_PORT 123
#define NTP_PACKET_SIZE 48

using namespace std;

int main(int argc, char* argv[]) {
    // Sunucu adresini al
    if (argc != 2) {
        cout << "Kullanim: " << argv[0] << " [NTP server IP/URL]" << endl;
        exit(1);
    }

    char* server = argv[1];

    // NTP sunucusuna bağlan
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd < 0) {
        perror("Soket acilamadi");
        exit(1);
    }

    struct hostent* host = gethostbyname(server);
    if (host == NULL) {
        perror("Sunucu bulunamadi");
        exit(1);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NTP_PORT);
    bcopy((char*)host->h_addr, (char*)&server_addr.sin_addr.s_addr, host->h_length);

    char ntp_packet[NTP_PACKET_SIZE];
    memset(ntp_packet, 0, NTP_PACKET_SIZE);
    ntp_packet[0] = 0x1b;

    // NTP sunucusuna istek gönder
    int n_sent = sendto(sock_fd, ntp_packet, NTP_PACKET_SIZE, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n_sent < 0) {
        perror("Istek gonderilemedi");
        exit(1);
    }

    // NTP sunucusundan yanıtı al
    char ntp_response[NTP_PACKET_SIZE];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    int n_recv = recvfrom(sock_fd, ntp_response, NTP_PACKET_SIZE, 0, (struct sockaddr*)&from_addr, &from_len);
    if (n_recv < 0) {
        perror("Yanit alinamadi");
        exit(1);
    }
    // Yanıtı işle ve yerel saat bilgisini ayarla
unsigned long long ntp_time = 0;
for (int i = 0; i < 8; i++) {
    ntp_time = (ntp_time << 8) | (unsigned long long)ntp_response[32 + i];
}

ntp_time -= 2208988800ULL; // 1900-01-01 00:00:00 -> 1970-01-01 00:00:00
time_t ntp_time_t = (time_t)ntp_time;
cout << "NTP sunucusunun saat bilgisi: " << ctime((const time_t*)&ntp_time_t);

time_t local_time = time(NULL);
cout << "Yerel saat bilgisi: " << ctime(&local_time);

time_t diff = ntp_time_t - local_time;
if (diff < 0) {
    cout << "Saat geriye dondu. Yerel saatiniz ayarlanamadi." << endl;
} else {
    cout << "Saat farki: " << diff << " saniye." << endl;
    cout << "Yeni saat bilgisi: " << ctime(&ntp_time_t);

    // Yerel saat bilgisini ayarla
    struct timeval tv;
    tv.tv_sec = ntp_time_t;
    tv.tv_usec = 0;
    if (settimeofday(&tv, NULL) < 0) {
        perror("Saat ayarlanamadi");
        exit(1);
    }
}

}