# C-Multithread-Chat

Bu proje, C programlama dili ve POSIX thread'leri kullanilarak gelistirilmis basit, terminal tabanli bir coklu istemci sohbet uygulamasidir.

## Ozellikler

* Coklu istemci destegi (POSIX threadler ile)
* Her istemci icin isim isteme ve atama
* Istemcilerin birbirlerine mesaj gondermesi
* Istemcilerin '/exit' komudu ile sohbetten ayrilabilmesi
* Sunucuda temel mesajlasma loglari

## Kullanilan Teknolojiler

* **Programlama Dili:** C
* **Ağ Programlama:** Soket Programlama (TCP/IP)
* **Eş Zamanlılık:** POSIX Threads (`pthread`)
* **Derleme Aracı:** GCC ve Makefile

## Kurulum ve Calistirma

### Gereksinimler

* GCC
* Make 
* POSIX uyumlu bir sistem(Linux,macOS vb.)

### Derleme 

1. Eski derlenmis dosyalari temizlemek icin
```bash
make clean
```
2. Projeyi derlemek icin
```bash
make
```
Bu komut 'bin/' dizini altinda 'Server' ve 'Client' adinda iki calistirilabilir dosya olusturacaktir.
### Calistirma

1.  **Sunucuyu Baslatma:**
    Ayri bir terminal acin ve proje kok dizininde asagidaki komudu calistirin:
    ```bash
    make run-server
    ```
    Veya dogrudan calistirilabilir dosyayi port numarasi ile baslatin.
    (Varsayilan olarak Makefile da 8080 portu uzerinden calisicaktir.)
    ```bash
    ./bin/Server <PORT_NUMARASI>
    # Ornegin: ./bin/Server 8080
    ```
2.  **Istemciyi Baslatma**
    Her istemci icin yeni bir terminal acin ve proje kok dizininde asagidaki komudu calistirin:
    ```bash
    make run-client
    ```
    Veya dogrudan calistirilabilir dosyayi sunucu port ve IP adresi ile calistirin.
    
    **Not:** Varsayilan olarak Makefile da 8080 portu ve 127.0.0.1 adresinde calisicaktir.
    ```bash
    ./bin/Client <SUNUCU_IP_ADRESI> <PORT_NUMARASI> 
    # Ornegin: ./bin/Client 127.0.0.1 8080
    ```
    Baglandiktan sonra sunucu sizden bir isim girmenizi isteyecektir. Isminizi girip Enter'a basin.
    
    Birden fazla istemci icin bu adimi tekrarlayin.

    **Not:** Istemciden cikmak icin istemci terminaline '/exit' yazip Enter'a basin.