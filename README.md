# DDoS-Tools

## Obiettivi

Vedendo le tipologie di attacchi ddod più diffuse il mio obiettivo è di simulare:

- Botnet che effettua un attacco diretto 
    - syn flood
- Reflection (attacchi che mirano al livello di rete)
    - DNS

Gli attacchi possono anche essere classificati come:

- attacchi che mirano al livello applicativo
    - syn flood
- attacchi che mirano al livello di rete
    - DNS reflection

## Soluzioni adottate

### Botnet

#### Syn flood
Per simulare la botnet posso usare ip spoofing per fare credere alla vittima che più client stiano mandando pacchetti syn contemporaneamente.
Inoltre posso usare un controllore e dei veri bot su più host per rafforzare la portata dell'attacco. 

### Reflection

#### DNS

Creo dei veri server DNS in rete locale usando dei container docker e li contatto con IP spoofed.

## Installazione

### Dipendenze

```
sudo apt-get install libpcap-dev libssl-dev cmake nlohmann-json3-dev
git clone https://github.com/mfontanini/libtins.git
cd libtins
mkdir build
cd build
cmake ../ -DLIBTINS_ENABLE_CXX11=1
make
sudo make install
sudo ldconfig
```

### Esecuzione

```
cd build
cmake DDoS-Tools ..
make
cp ../settings.json .
sudo DDoS_Tools -{mode} {TARGET_IP} {opt TARGET_PORT}
```

### DNS Server

Per creare un server DNS bisogna copiare la cartella `dns_server` sull'host ed eseguire il comando `sudo docker-compose up -d`.