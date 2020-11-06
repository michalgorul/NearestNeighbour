;
; Temat: Algorytm najbli�szego s�siedztwa zmieniaj�cy wielko�� zdj�cia
;
; Opis: Algorytm najbli�szego s�siedzta (ang. Nearest Neighbour) jest najprostszym algorytmem zmiany wielko�ci obrazu
;       (ang. resampling). Ka�dy piksel obrazu wynikowego przyjmuje niezmodyfikowan� warto�� piksela obrazu wej�ciowego 
;        po�o�onego najbli�ej aktualnie rozpatrywanego punktu.
;
; Autor: Micha� G�ral semestr 5 rok 2020/2021
;
; Historia zmian:
;
; Wersja: 0.1
;   dodane zmienne lokalne
;
; Wersja: 0.2
;   dodane obliczanie skali poziomej i pionowej oraz zapis tych warto�ci do zmiennyh lokalnych
;
; Wersja: 0.3
;   pocz�tki tworzenia dw�ch p�tli
;
; Wersja: 0.4
;   poprawne dzia�anie p�tli
;
; Wersja: 0.5
;   pocz�tki obliczania indeks�w obraz�w wej�ciowego i wyj�ciowego
;
; Wersja: 0.6
;   obliczanie indeks�w dzia�� poprawnie
;
; Wersja: 0.7
;   przypisane warto�ci piksela obrazu wej�ciowego po�o�onego najbli�ej aktualnie rozpatrywanego punktu w obrazie
;   wyj�ciowym dzia�� poprawnie.
;
; Wersja: 0.8
;   przyst�pienie do obs�ugi w�tk�w
;
; Wersja: 0.9
;   obs�uga w�tk�w dzia�a poprawnie
;
; Wersja: 1.0
;   wersja w pe�ni dzia�aj�ca

.data

.code

; Parametry wej�ciowe:
;   do procedury przekazuj� struktur�(adres na jej pierwszy element) z potrzebnymi parametrametrami oraz dwa inty potrzebne do obs�ugi w�tk�w
;   pierwszy parametr funkcji zapisywany jest w rejestrze rcx 
;   drugi parametr funkcji zapisywany jest w rejestrze rdx, jest to warto�� iIn, mniejsze od heightOut i wi�ksze od 0
;   trzeci parametr funkcji zapisywany jest w rejestrze r8, jest to warto�� iOut, mniejsze od heightOut i wi�ksze od 0
;   warto�� DWORD PTR [rcx] to seroko�� obrazu wej�ciowego, widthIn
;   warto�� DWORD PTR [rcx+4] to wysoko�� obrazu wej�ciowego, heightIn
;   warto�� DWORD PTR [rcx+8] to seroko�� obrazu wej�ciowego, widthOut
;   warto�� DWORD PTR [rcx+12] to wysoko�� obrazu wej�ciowego, heightOut
;   warto�� QWORD PTR [rcx+16] to adres na pierwszy element obrazu wej�ciowego, dataIn
;   warto�� QWORD PTR [rcx+24] to adres na pierwszy element obrazu wyj�ciowego, dataOut
;
; Parametry wyj�ciowe:
;   procedura zamienia warto�ci w odpowiednich miejscach pami�ci i nic nie zwraca
;
; Zniszczone rejestry:
;   eax, edx, rax, r10, r11

nearestNeighbourAsm proc 

    ; zmienne lokalne s� przechowywane na stosie
    LOCAL ratioX: REAL4	            ; w tej zmiennej lokalnej b�d� przechowywa� stosunek X (float)
    LOCAL ratioY: REAL4	            ; w tej zmiennej lokalnej b�d� przechowywa� stosunek Y (float)
    LOCAL indexSource: DWORD        ; w tej zmiennej lokalnej b�d� przechowywa� indeks pliku wej�ciowego (int)
    LOCAL indexOut: DWORD           ; w tej zmiennej lokalnej b�d� przechowywa� indeks pliku wyj�ciowego (int)
    LOCAL outerIterator: DWORD      ; w tej zmiennej lokalnej b�d� przechowywa� iterator po p�tli zewn�trznej (int)
    LOCAL innerIterator: DWORD      ; w tej zmiennej lokalnej b�d� przechowywa� iterator po p�tli wewn�trznej (int)

    push    rbp                     ; odk�adam na stosie warto�� rejestru rbp 
    mov     rbp, rsp                ; nadpisuje warto�� rbp obecn� warto�ci� rsp (warto�� wska�nika stosu)

    ; b�d� teraz oblicza� ratioX
    pxor        xmm0, xmm0                  ; w xmm0 przechowam licznik, czyli szeroko�� wej�ciow� (ustawim wszystkie bity w rejestrze xmm0 na zero)
    cvtsi2sd    xmm0, DWORD PTR [rcx]       ; zamieniam 32-bitow� liczb� ca�kowit� (widthIn) na liczb� zmiennopozycyjn� podw�jnej precyzji.
    pxor        xmm1, xmm1                  ; w xmm1 przechowam mianownik, czyli szeroko�� wyj�ciow� (widthOut) (ustawim wszystkie bity w rejestrze xmm1 na zero)
    cvtsi2sd    xmm1, DWORD PTR [rcx+8]     ; zamieniam 32-bitow� liczb� ca�kowit� (widthOut) na liczb� zmiennopozycyjn� podw�jnej precyzji.
    divsd       xmm0, xmm1                  ; dziel� liczb� podw�jnej precyzji przechowywan� w xmm0 przez liczb� podw�jnej precyzji przechowywan� w xmm1
                                            ; i zapisuj� wynik w rejestrze xmm0
    cvtsd2ss    xmm0, xmm0                  ; zamieniam liczb� zmiennopozycyjn� podw�jnej precyzji(64bit) na liczb� zmiennopozycyjn� pojedynczej precyzji(32bit) i zapisuj� w xmm0
    movss       ratioX, xmm0                ; przepisuj� wynik dzielenia do do zmiennej lokalnej ratioX

    ; b�d� teraz oblicza� ratioY
    pxor        xmm0, xmm0                  ; w xmm0 przechowam licznik, czyli wysoko�� wej�ciow� (ustawim wszystkie bity w rejestrze xmm0 na zero)
    cvtsi2sd    xmm0, DWORD PTR [rcx+4]     ; zamieniam 32-bitow� liczb� ca�kowit� (heightIn) na liczb� zmiennopozycyjn� podw�jnej precyzji.
    pxor        xmm1, xmm1                  ; w xmm1 przechowam mianownik, czyli wysoko�� wyj�ciow� (heightOut) (ustawim wszystkie bity w rejestrze xmm1 na zero)
    cvtsi2sd    xmm1, DWORD PTR [rcx+12]    ; zamieniam 32-bitow� liczb� ca�kowit� (heightOut) na liczb� zmiennopozycyjn� podw�jnej precyzji.
    divsd       xmm0, xmm1                  ; dziel� liczb� podw�jnej precyzji przechowywan� w xmm0 przez liczb� podw�jnej precyzji przechowywan� w xmm1
                                            ; i zapisuj� wynik w rejestrze xmm0
    cvtsd2ss    xmm0, xmm0                  ; zamieniam liczb� zmiennopozycyjn� podw�jnej precyzji(64bit) na liczb� zmiennopozycyjn� pojedynczej precyzji(32bit) i zapisuj� w xmm0
    movss       ratioY, xmm0                ; przepisuj� wynik dzielenia do zmiennej lokalnej ratioY

    mov         outerIterator, edx          ; ustawiam warto�� iteratora p�tli zewn�trznej zgodnie z odpowiadaj�cej warto�ci danego w�tka

OUTER_LOOP:                             ; p�tla zewn�trzna
    mov     eax, outerIterator          ; przepisuje warto�� iteratora p�tli zewn�trznej do rejestru eax
    cmp     eax, r8d                    ; por�wnuj� iterator z warto�ci� iOut z odpowiadaj�cej warto�ci danego w�tka
    jge     OUTER_LOOP_END              ; je�eli iterator jest wi�kszy lub r�wny wysoko�ci obrazu wyj�ciowego to wychodz� z p�tli

    mov     innerIterator, 0            ; zeruje iterator p�tli wewn�trznej, b�dzie on zapisany w zmiennej lokalnej innerIterator

INNER_LOOP:                             ; p�tla wewn�trzna
    mov     eax, innerIterator          ; przepisuje warto�� iteratora p�tli wewn�trznej do rejestru eax
    cmp     eax, DWORD PTR [rcx+8]      ; por�wnuj� iterator z warto�ci� widthOut
    jge     INNER_LOOP_END              ; je�eli iterator jest wi�kszy lub r�wny szeroko�ci obrazu wyj�ciowego to wychodz� z p�tli

    ; b�d� teraz oblicza� indeks w tablicy warto�ci pikseli obrazu wej�ciowego
    pxor        xmm0, xmm0                  ; ustawim wszystkie bity w rejestrze xmm0 na zero
    cvtsi2ss    xmm0, outerIterator         ; zamieniam 32-bitow� liczb� ca�kowit� (iterator p�tli zewn�trznej) na liczb� zmiennopozycyjn� pojedynczej precyzji.
    mulss       xmm0, ratioY                ; wykonuj� mno�enie warto�ci iteratora p�tli zewn�trznej i warto�ci ratioY, wynik zapisuj� w xmm0
    cvttss2si   eax, xmm0                   ; zamieniam wynik mno�enia na liczb� ca�kowit�, dzia�a jak rzutowanie na int w cpp.
    imul        eax, [rcx]                  ; mno�� t� liczb� ca�kowit� razy szeroko�� obrazu wej�ciowego
    mov         edx, eax                    ; przepisuj� warto�� tego mno�enia do rejestru edx
    pxor        xmm0, xmm0                  ; ustawim wszystkie bity w rejestrze xmm0 na zero
    cvtsi2ss    xmm0, innerIterator         ; zamieniam 32-bitow� liczb� ca�kowit� (iterator p�tli wewn�trznej) na liczb� zmiennopozycyjn� pojedynczej precyzji.
    mulss       xmm0, ratioX                ; wykonuj� mno�enie warto�ci iteratora p�tli wewn�trznej i warto�ci ratioX, wynik zapisuj� w xmm0
    cvttss2si   eax, xmm0                   ; zamieniam wynik mno�enia na liczb� ca�kowit�, dzia�a jak rzutowanie na int w cpp.
    add         edx, eax                    ; dodaje do siebie wyniki tych dzia�a� ((int)(i * ratioY) * widthIn + (int)(j * ratioX)))
    mov         eax, edx                    ; sum� t� przepisuj� do rejestru eax, w rejestrze edx ta warto�� b�dzie mi jeszcze potrzebna
    add         eax, eax                    ; wynik sumy musz� pomno�y� x3, ale szybciej jest doda� t� warto�� dwa razy, pierwsze dodanie
    add         eax, edx                    ; drugie dodanie, w rejestrze edx dalej znajduje si� wynik dzia�ania ((int)(i * ratioY) * widthIn + (int)(j * ratioX)))
    mov         indexSource, eax            ; przepisuj� wynik dzia�ania -((int)(i * ratioY) * widthIn + (int)(j * ratioX)) * 3 do indexSource
    ; indeks w tablicy warto�ci pikseli obrazu wej�ciowego jest ju� poprawnie umieszczony w indexSource

    ; b�d� teraz oblicza� indeks w tablicy warto�ci pikseli obrazu wyj�ciowego
    mov     eax, outerIterator          ; przepisuj� warto�� iteratora p�tli zewn�trznej do rejestru eax
    imul    eax, DWORD PTR [rcx+8]      ; mno�� warto�� iteratora p�tli zewn�trznej i warto�� widthOut i wynik zapisuj� w rejestrze eax 
    mov     edx, eax                    ; przepisuj� warto�� rejestru eax do rejestru edx
    mov     eax, innerIterator          ; przepisuj� warto�� iteratora p�tli wewn�trznej do rejestru eax
    add     edx, eax                    ; dodaj� do siebie wyniki tych dzia�a� (i * widthOut + j) i wynik zapisuj� w rejestrze edx 
    mov     eax, edx                    ; sum� t� przepisuj� do rejestru eax, w rejestrze edx ta warto�� b�dzie mi jeszcze potrzebna 
    add     eax, eax                    ; wynik sumy musz� pomno�y� x3, ale szybciej jest doda� t� warto�� dwa razy, pierwsze dodanie
    add     eax, edx                    ; drugie dodanie, w rejestrze edx dalej znajduje si� wynik dzia�ania (i * widthOut + j)
    mov     indexOut, eax               ; przepisuj� wynik dzia�ania - (i * widthOut + j) * 3 do indexOut
    ; indeks w tablicy warto�ci pikseli obrazu wyj�ciowego jest ju� poprawnie umieszczony w indexOut

    ; teraz zajm� si� ustawieniem odpowiedniej warto�ci koloru czerwonego piksela w odpowiednim miejscu w obrazie wyj�ciowym
    mov     eax, indexSource            ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wej�ciowego do rejestru eax
    movsxd  r11, eax                    ; przepisuj� warto�� rejestru eax do rejestru r11 (movsxd, poniewa� eax jest 32bitowy, a r11 64 bitowy)
    mov     rax, QWORD PTR [rcx+16]     ; przepisuj� warto�� z adresu[rcx+16] (adres pierwszego elementu obrazu wej�ciowego) do rejestru rax
    add     rax, r11                    ; dodaj� do warto�ci rejestru rax warto�� r11, by by� w odpowiednim miejscu w obrazie wej�ciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wej�ciowego
    mov     edx, indexOut               ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wyj�ciowego do rejestru edx
    movsxd  r10, edx                    ; przepisuj� warto�� rejestru edx do rejestru r10 (movsxd, poniewa� edx jest 32bitowy, a r10 64 bitowy)
    mov     r11, QWORD PTR [rcx+24]     ; przepisuj� warto�� [rcx+24] (adres pierwszego elementu obrazu wyj�ciowego) do rejestru r11
    add     r11, r10                    ; dodaj� do warto�ci rejestru r11 warto�� r10, by by� w odpowiednim miejscu w obrazie wyj�ciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyj�ciowego
    movzx   eax, BYTE PTR [rax]         ; przepisuj� warto�� z adresu [rax], czyli warto�� odpowiedniego piksela w obrazie wej�ciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisuj� rejestr al (8 bit�w z rejestru eax) do warto�ci z odpowiedniego piksela w obrazie wyj�ciowym

    ; teraz zajm� si� ustawieniem odpowiedniej warto�ci koloru zielonego piksela w odpowiednim miejscu w obrazie wyj�ciowym
    mov     eax, indexSource            ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wej�ciowego do rejestru eax
    cdqe                                ; rozszerzam warto�� DWORD (warto�� 32-bitowa) w rejestrze eax do warto�ci QWORD (warto�� 64-bitowa) w rejestrze rax.
    lea     r11, [rax+1]                ; wczytuje efektywny adres rax+1, czyli warto�� sk�adowej zielonej obrazu wej�ciowego, do r11
    mov     rax, QWORD PTR [rcx+16]     ; przepisuj� warto�� z adresu [rcx+16] (adres pierwszego elementu obrazu wej�ciowego) do rejestru rax 
    add     rax, r11                    ; dodaj� do warto�ci rejestru rax warto�� r11, by by� w odpowiednim miejscu w obrazie wej�ciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wej�ciowego
    mov     edx, indexOut               ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wyj�ciowego do rejestru edx
    movsxd  r11, edx                    ; przepisuj� warto�� rejestru edx do rejestru rcx (movsxd, poniewa� edx jest 32bitowy, a rcx 64 bitowy)
    lea     r10, [r11+1]                ; wczytuje efektywny adres r11+1, czyli warto�� sk�adowej zielonej obrazu wej�ciowego, do r10
    mov     r11, QWORD PTR [rcx+24]     ; przepisuj� warto�� [rcx+24] (adres pierwszego elementu obrazu wyj�ciowego) do rejestru r11
    add     r11, r10                    ; dodaj� do warto�ci rejestru r11 warto�� r10, by by� w odpowiednim miejscu w obrazie wyj�ciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyj�ciowego
    movzx   eax, BYTE PTR [rax]         ; przepisuj� warto�� z adresu [rax], czyli warto�� odpowiedniego piksela w obrazie wej�ciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisuj� rejestr al (8 bit�w z rejestru eax) do warto�ci z odpowiedniego piksela w obrazie wyj�ciowym

    ; teraz zajm� si� ustawieniem odpowiedniej warto�ci koloru niebieskiego piksela w odpowiednim miejscu w obrazie wyj�ciowym
    mov     eax, indexSource            ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wej�ciowego do rejestru eax
    cdqe                                ; rozszerzam warto�� DWORD (warto�� 32-bitowa) w rejestrze eax do warto�ci QWORD (warto�� 64-bitowa) w rejestrze rax.
    lea     r11, [rax+2]                ; wczytuje efektywny adres rax+2, czyli warto�� sk�adowej niebieskiej obrazu wej�ciowego, do r11
    mov     rax, QWORD PTR [rcx+16]     ; przepisuj� warto�� z adresu [rcx+16] (adres pierwszego elementu obrazu wej�ciowego) do rejestru rax 
    add     rax, r11                    ; dodaj� do warto�ci rejestru rax warto�� r11, by by� w odpowiednim miejscu w obrazie wej�ciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wej�ciowego
    mov     edx, indexOut               ; przepisuj� indeks w tablicy warto�ci sk�adowej czerwonej piksela obrazu wyj�ciowego do rejestru edx
    movsxd  r11, edx                    ; przepisuj� warto�� rejestru edx do rejestru rcx (movsxd, poniewa� edx jest 32bitowy, a rcx 64 bitowy)
    lea     r10, [r11+2]                ; wczytuje efektywny adres r11+2, czyli warto�� sk�adowej niebieskiej obrazu wej�ciowego, do r10
    mov     r11, QWORD PTR [rcx+24]     ; przepisuj� warto�� [rcx+24] (adres pierwszego elementu obrazu wyj�ciowego) do rejestru r11
    add     r11, r10                    ; dodaj� do warto�ci rejestru r11 warto�� r10, by by� w odpowiednim miejscu w obrazie wyj�ciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyj�ciowego
    movzx   eax, BYTE PTR [rax]         ; przepisuj� warto�� z adresu [rax], czyli warto�� odpowiedniego piksela w obrazie wej�ciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisuj� rejestr al (8 bit�w z rejestru eax) do warto�ci z odpowiedniego piksela w obrazie wyj�ciowym
    ; w tej iteracji warto�ci pikseli w obrazie wyj�ciowym zosta�y zamienione

    add     innerIterator, 1        ; do iteratora p�tli wewn�trznej dodaj� 1
    jmp     INNER_LOOP              ; skacze bezwarunkowo do pocz�tku p�tli wewn�trznej

INNER_LOOP_END:
    add     outerIterator, 1        ; do iteratora p�tli zewn�trznej dodaj� 1
    jmp     OUTER_LOOP              ; skacze bezwarunkowo do pocz�tku p�tli zewn�trznej

OUTER_LOOP_END:

    pop     rbp         ; zdejmuje ze szczytu stosu warto�� i przepisuj� j� do rbp
    ret                 ; powr�t z procedury

nearestNeighbourAsm endp 
end