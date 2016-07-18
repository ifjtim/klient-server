Dokumentace IPK

IPK Program pro vyhledání informací o uživatelích Unixového OS

jméno: Tomasz Konderla

login: xkonde03

projekt: 1 projekt IPK

Ukol:
komunikacé klient-server ,vyhkedani informaci v passwd

Stručky popis:
realizoval jsem komunikaci server-klient. Program má za úkol klientovi zaslat udaje které si vyžádá. Program
využívá komunikaci pomoci soketu.
Zasílané správy:
Server klientovi odpovídá na jednotlivé správy dokud mu klient nepošle správu o ukončeni komunikace je to z
důvodu vyhledáváni vice položek. Klient postupně posila jednotlivé požadavky, správy , posila je pouze po jedné
položce(jeden login nebo uid), až jak dostane odpověď pošle další hledanou položku na konec pošle správu o
ukončeni komunikace.
Server pak čeká na další komunikací. Server muže najednou obsluhovat více klie ntu.

Vyhled sprav:
Správa od klienta - co požaduje (login,uid) konkretní login
jaký druh informaci hledá
Ukončující správa - konec konec konec
Správa od serveru - informace o nalezeni (ok)
konkretní informace
chybová správa od serveru- chyba: znění chybové hlašky
Ukončující správa - konec

Konkretní příklad:
. /client -h eva.fit.vutbr.cz -l xkonde03 unknown -p 55555 -L -HS
klient:
odpověď server:
Klient:
odpověď server:
Klient:
odpověď server:
vystup programu client:
login xkonde03
LHS
ok
xkonde03 /homes/kazi/xkonde03 /homes/kazi/rabj
login unknown
LHS
Chyba: neznamy login unknown
konec konec konec
konec
xkonde03 /homes/eva/xk/xkonde03 /bin/ksh
Chyba: neznamy login unknown
