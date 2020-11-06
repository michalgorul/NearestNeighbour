;
; Temat: Algorytm najbli¿szego s¹siedztwa zmieniaj¹cy wielkoœæ zdjêcia
;
; Opis: Algorytm najbli¿szego s¹siedzta (ang. Nearest Neighbour) jest najprostszym algorytmem zmiany wielkoœci obrazu
;       (ang. resampling). Ka¿dy piksel obrazu wynikowego przyjmuje niezmodyfikowan¹ wartoœæ piksela obrazu wejœciowego 
;        po³o¿onego najbli¿ej aktualnie rozpatrywanego punktu.
;
; Autor: Micha³ Góral semestr 5 rok 2020/2021
;
; Historia zmian:
;
; Wersja: 0.1
;   dodane zmienne lokalne
;
; Wersja: 0.2
;   dodane obliczanie skali poziomej i pionowej oraz zapis tych wartoœci do zmiennyh lokalnych
;
; Wersja: 0.3
;   pocz¹tki tworzenia dwóch pêtli
;
; Wersja: 0.4
;   poprawne dzia³anie pêtli
;
; Wersja: 0.5
;   pocz¹tki obliczania indeksów obrazów wejœciowego i wyjœciowego
;
; Wersja: 0.6
;   obliczanie indeksów dzia³¹ poprawnie
;
; Wersja: 0.7
;   przypisane wartoœci piksela obrazu wejœciowego po³o¿onego najbli¿ej aktualnie rozpatrywanego punktu w obrazie
;   wyjœciowym dzia³¹ poprawnie.
;
; Wersja: 0.8
;   przyst¹pienie do obs³ugi w¹tków
;
; Wersja: 0.9
;   obs³uga w¹tków dzia³a poprawnie
;
; Wersja: 1.0
;   wersja w pe³ni dzia³aj¹ca

.data

.code

; Parametry wejœciowe:
;   do procedury przekazujê strukturê(adres na jej pierwszy element) z potrzebnymi parametrametrami oraz dwa inty potrzebne do obs³ugi w¹tków
;   pierwszy parametr funkcji zapisywany jest w rejestrze rcx 
;   drugi parametr funkcji zapisywany jest w rejestrze rdx, jest to wartoœæ iIn, mniejsze od heightOut i wiêksze od 0
;   trzeci parametr funkcji zapisywany jest w rejestrze r8, jest to wartoœæ iOut, mniejsze od heightOut i wiêksze od 0
;   wartoœæ DWORD PTR [rcx] to serokoœæ obrazu wejœciowego, widthIn
;   wartoœæ DWORD PTR [rcx+4] to wysokoœæ obrazu wejœciowego, heightIn
;   wartoœæ DWORD PTR [rcx+8] to serokoœæ obrazu wejœciowego, widthOut
;   wartoœæ DWORD PTR [rcx+12] to wysokoœæ obrazu wejœciowego, heightOut
;   wartoœæ QWORD PTR [rcx+16] to adres na pierwszy element obrazu wejœciowego, dataIn
;   wartoœæ QWORD PTR [rcx+24] to adres na pierwszy element obrazu wyjœciowego, dataOut
;
; Parametry wyjœciowe:
;   procedura zamienia wartoœci w odpowiednich miejscach pamiêci i nic nie zwraca
;
; Zniszczone rejestry:
;   eax, edx, rax, r10, r11

nearestNeighbourAsm proc 

    ; zmienne lokalne s¹ przechowywane na stosie
    LOCAL ratioX: REAL4	            ; w tej zmiennej lokalnej bêdê przechowywa³ stosunek X (float)
    LOCAL ratioY: REAL4	            ; w tej zmiennej lokalnej bêdê przechowywa³ stosunek Y (float)
    LOCAL indexSource: DWORD        ; w tej zmiennej lokalnej bêdê przechowywa³ indeks pliku wejœciowego (int)
    LOCAL indexOut: DWORD           ; w tej zmiennej lokalnej bêdê przechowywa³ indeks pliku wyjœciowego (int)
    LOCAL outerIterator: DWORD      ; w tej zmiennej lokalnej bêdê przechowywa³ iterator po pêtli zewnêtrznej (int)
    LOCAL innerIterator: DWORD      ; w tej zmiennej lokalnej bêdê przechowywa³ iterator po pêtli wewnêtrznej (int)

    push    rbp                     ; odk³adam na stosie wartoœæ rejestru rbp 
    mov     rbp, rsp                ; nadpisuje wartoœæ rbp obecn¹ wartoœci¹ rsp (wartoœæ wskaŸnika stosu)

    ; bêdê teraz oblicza³ ratioX
    pxor        xmm0, xmm0                  ; w xmm0 przechowam licznik, czyli szerokoœæ wejœciow¹ (ustawim wszystkie bity w rejestrze xmm0 na zero)
    cvtsi2sd    xmm0, DWORD PTR [rcx]       ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (widthIn) na liczbê zmiennopozycyjn¹ podwójnej precyzji.
    pxor        xmm1, xmm1                  ; w xmm1 przechowam mianownik, czyli szerokoœæ wyjœciow¹ (widthOut) (ustawim wszystkie bity w rejestrze xmm1 na zero)
    cvtsi2sd    xmm1, DWORD PTR [rcx+8]     ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (widthOut) na liczbê zmiennopozycyjn¹ podwójnej precyzji.
    divsd       xmm0, xmm1                  ; dzielê liczbê podwójnej precyzji przechowywan¹ w xmm0 przez liczbê podwójnej precyzji przechowywan¹ w xmm1
                                            ; i zapisujê wynik w rejestrze xmm0
    cvtsd2ss    xmm0, xmm0                  ; zamieniam liczbê zmiennopozycyjn¹ podwójnej precyzji(64bit) na liczbê zmiennopozycyjn¹ pojedynczej precyzji(32bit) i zapisujê w xmm0
    movss       ratioX, xmm0                ; przepisujê wynik dzielenia do do zmiennej lokalnej ratioX

    ; bêdê teraz oblicza³ ratioY
    pxor        xmm0, xmm0                  ; w xmm0 przechowam licznik, czyli wysokoœæ wejœciow¹ (ustawim wszystkie bity w rejestrze xmm0 na zero)
    cvtsi2sd    xmm0, DWORD PTR [rcx+4]     ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (heightIn) na liczbê zmiennopozycyjn¹ podwójnej precyzji.
    pxor        xmm1, xmm1                  ; w xmm1 przechowam mianownik, czyli wysokoœæ wyjœciow¹ (heightOut) (ustawim wszystkie bity w rejestrze xmm1 na zero)
    cvtsi2sd    xmm1, DWORD PTR [rcx+12]    ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (heightOut) na liczbê zmiennopozycyjn¹ podwójnej precyzji.
    divsd       xmm0, xmm1                  ; dzielê liczbê podwójnej precyzji przechowywan¹ w xmm0 przez liczbê podwójnej precyzji przechowywan¹ w xmm1
                                            ; i zapisujê wynik w rejestrze xmm0
    cvtsd2ss    xmm0, xmm0                  ; zamieniam liczbê zmiennopozycyjn¹ podwójnej precyzji(64bit) na liczbê zmiennopozycyjn¹ pojedynczej precyzji(32bit) i zapisujê w xmm0
    movss       ratioY, xmm0                ; przepisujê wynik dzielenia do zmiennej lokalnej ratioY

    mov         outerIterator, edx          ; ustawiam wartoœæ iteratora pêtli zewnêtrznej zgodnie z odpowiadaj¹cej wartoœci danego w¹tka

OUTER_LOOP:                             ; pêtla zewnêtrzna
    mov     eax, outerIterator          ; przepisuje wartoœæ iteratora pêtli zewnêtrznej do rejestru eax
    cmp     eax, r8d                    ; porównujê iterator z wartoœci¹ iOut z odpowiadaj¹cej wartoœci danego w¹tka
    jge     OUTER_LOOP_END              ; je¿eli iterator jest wiêkszy lub równy wysokoœci obrazu wyjœciowego to wychodzê z pêtli

    mov     innerIterator, 0            ; zeruje iterator pêtli wewnêtrznej, bêdzie on zapisany w zmiennej lokalnej innerIterator

INNER_LOOP:                             ; pêtla wewnêtrzna
    mov     eax, innerIterator          ; przepisuje wartoœæ iteratora pêtli wewnêtrznej do rejestru eax
    cmp     eax, DWORD PTR [rcx+8]      ; porównujê iterator z wartoœci¹ widthOut
    jge     INNER_LOOP_END              ; je¿eli iterator jest wiêkszy lub równy szerokoœci obrazu wyjœciowego to wychodzê z pêtli

    ; bêdê teraz oblicza³ indeks w tablicy wartoœci pikseli obrazu wejœciowego
    pxor        xmm0, xmm0                  ; ustawim wszystkie bity w rejestrze xmm0 na zero
    cvtsi2ss    xmm0, outerIterator         ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (iterator pêtli zewnêtrznej) na liczbê zmiennopozycyjn¹ pojedynczej precyzji.
    mulss       xmm0, ratioY                ; wykonujê mno¿enie wartoœci iteratora pêtli zewnêtrznej i wartoœci ratioY, wynik zapisujê w xmm0
    cvttss2si   eax, xmm0                   ; zamieniam wynik mno¿enia na liczbê ca³kowit¹, dzia³a jak rzutowanie na int w cpp.
    imul        eax, [rcx]                  ; mno¿ê tê liczbê ca³kowit¹ razy szerokoœæ obrazu wejœciowego
    mov         edx, eax                    ; przepisujê wartoœæ tego mno¿enia do rejestru edx
    pxor        xmm0, xmm0                  ; ustawim wszystkie bity w rejestrze xmm0 na zero
    cvtsi2ss    xmm0, innerIterator         ; zamieniam 32-bitow¹ liczbê ca³kowit¹ (iterator pêtli wewnêtrznej) na liczbê zmiennopozycyjn¹ pojedynczej precyzji.
    mulss       xmm0, ratioX                ; wykonujê mno¿enie wartoœci iteratora pêtli wewnêtrznej i wartoœci ratioX, wynik zapisujê w xmm0
    cvttss2si   eax, xmm0                   ; zamieniam wynik mno¿enia na liczbê ca³kowit¹, dzia³a jak rzutowanie na int w cpp.
    add         edx, eax                    ; dodaje do siebie wyniki tych dzia³añ ((int)(i * ratioY) * widthIn + (int)(j * ratioX)))
    mov         eax, edx                    ; sumê tê przepisujê do rejestru eax, w rejestrze edx ta wartoœæ bêdzie mi jeszcze potrzebna
    add         eax, eax                    ; wynik sumy muszê pomno¿yæ x3, ale szybciej jest dodaæ tê wartoœæ dwa razy, pierwsze dodanie
    add         eax, edx                    ; drugie dodanie, w rejestrze edx dalej znajduje siê wynik dzia³ania ((int)(i * ratioY) * widthIn + (int)(j * ratioX)))
    mov         indexSource, eax            ; przepisujê wynik dzia³ania -((int)(i * ratioY) * widthIn + (int)(j * ratioX)) * 3 do indexSource
    ; indeks w tablicy wartoœci pikseli obrazu wejœciowego jest ju¿ poprawnie umieszczony w indexSource

    ; bêdê teraz oblicza³ indeks w tablicy wartoœci pikseli obrazu wyjœciowego
    mov     eax, outerIterator          ; przepisujê wartoœæ iteratora pêtli zewnêtrznej do rejestru eax
    imul    eax, DWORD PTR [rcx+8]      ; mno¿ê wartoœæ iteratora pêtli zewnêtrznej i wartoœæ widthOut i wynik zapisujê w rejestrze eax 
    mov     edx, eax                    ; przepisujê wartoœæ rejestru eax do rejestru edx
    mov     eax, innerIterator          ; przepisujê wartoœæ iteratora pêtli wewnêtrznej do rejestru eax
    add     edx, eax                    ; dodajê do siebie wyniki tych dzia³añ (i * widthOut + j) i wynik zapisujê w rejestrze edx 
    mov     eax, edx                    ; sumê tê przepisujê do rejestru eax, w rejestrze edx ta wartoœæ bêdzie mi jeszcze potrzebna 
    add     eax, eax                    ; wynik sumy muszê pomno¿yæ x3, ale szybciej jest dodaæ tê wartoœæ dwa razy, pierwsze dodanie
    add     eax, edx                    ; drugie dodanie, w rejestrze edx dalej znajduje siê wynik dzia³ania (i * widthOut + j)
    mov     indexOut, eax               ; przepisujê wynik dzia³ania - (i * widthOut + j) * 3 do indexOut
    ; indeks w tablicy wartoœci pikseli obrazu wyjœciowego jest ju¿ poprawnie umieszczony w indexOut

    ; teraz zajmê siê ustawieniem odpowiedniej wartoœci koloru czerwonego piksela w odpowiednim miejscu w obrazie wyjœciowym
    mov     eax, indexSource            ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wejœciowego do rejestru eax
    movsxd  r11, eax                    ; przepisujê wartoœæ rejestru eax do rejestru r11 (movsxd, poniewa¿ eax jest 32bitowy, a r11 64 bitowy)
    mov     rax, QWORD PTR [rcx+16]     ; przepisujê wartoœæ z adresu[rcx+16] (adres pierwszego elementu obrazu wejœciowego) do rejestru rax
    add     rax, r11                    ; dodajê do wartoœci rejestru rax wartoœæ r11, by byæ w odpowiednim miejscu w obrazie wejœciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wejœciowego
    mov     edx, indexOut               ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wyjœciowego do rejestru edx
    movsxd  r10, edx                    ; przepisujê wartoœæ rejestru edx do rejestru r10 (movsxd, poniewa¿ edx jest 32bitowy, a r10 64 bitowy)
    mov     r11, QWORD PTR [rcx+24]     ; przepisujê wartoœæ [rcx+24] (adres pierwszego elementu obrazu wyjœciowego) do rejestru r11
    add     r11, r10                    ; dodajê do wartoœci rejestru r11 wartoœæ r10, by byæ w odpowiednim miejscu w obrazie wyjœciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyjœciowego
    movzx   eax, BYTE PTR [rax]         ; przepisujê wartoœæ z adresu [rax], czyli wartoœæ odpowiedniego piksela w obrazie wejœciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisujê rejestr al (8 bitów z rejestru eax) do wartoœci z odpowiedniego piksela w obrazie wyjœciowym

    ; teraz zajmê siê ustawieniem odpowiedniej wartoœci koloru zielonego piksela w odpowiednim miejscu w obrazie wyjœciowym
    mov     eax, indexSource            ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wejœciowego do rejestru eax
    cdqe                                ; rozszerzam wartoœæ DWORD (wartoœæ 32-bitowa) w rejestrze eax do wartoœci QWORD (wartoœæ 64-bitowa) w rejestrze rax.
    lea     r11, [rax+1]                ; wczytuje efektywny adres rax+1, czyli wartoœæ sk³adowej zielonej obrazu wejœciowego, do r11
    mov     rax, QWORD PTR [rcx+16]     ; przepisujê wartoœæ z adresu [rcx+16] (adres pierwszego elementu obrazu wejœciowego) do rejestru rax 
    add     rax, r11                    ; dodajê do wartoœci rejestru rax wartoœæ r11, by byæ w odpowiednim miejscu w obrazie wejœciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wejœciowego
    mov     edx, indexOut               ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wyjœciowego do rejestru edx
    movsxd  r11, edx                    ; przepisujê wartoœæ rejestru edx do rejestru rcx (movsxd, poniewa¿ edx jest 32bitowy, a rcx 64 bitowy)
    lea     r10, [r11+1]                ; wczytuje efektywny adres r11+1, czyli wartoœæ sk³adowej zielonej obrazu wejœciowego, do r10
    mov     r11, QWORD PTR [rcx+24]     ; przepisujê wartoœæ [rcx+24] (adres pierwszego elementu obrazu wyjœciowego) do rejestru r11
    add     r11, r10                    ; dodajê do wartoœci rejestru r11 wartoœæ r10, by byæ w odpowiednim miejscu w obrazie wyjœciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyjœciowego
    movzx   eax, BYTE PTR [rax]         ; przepisujê wartoœæ z adresu [rax], czyli wartoœæ odpowiedniego piksela w obrazie wejœciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisujê rejestr al (8 bitów z rejestru eax) do wartoœci z odpowiedniego piksela w obrazie wyjœciowym

    ; teraz zajmê siê ustawieniem odpowiedniej wartoœci koloru niebieskiego piksela w odpowiednim miejscu w obrazie wyjœciowym
    mov     eax, indexSource            ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wejœciowego do rejestru eax
    cdqe                                ; rozszerzam wartoœæ DWORD (wartoœæ 32-bitowa) w rejestrze eax do wartoœci QWORD (wartoœæ 64-bitowa) w rejestrze rax.
    lea     r11, [rax+2]                ; wczytuje efektywny adres rax+2, czyli wartoœæ sk³adowej niebieskiej obrazu wejœciowego, do r11
    mov     rax, QWORD PTR [rcx+16]     ; przepisujê wartoœæ z adresu [rcx+16] (adres pierwszego elementu obrazu wejœciowego) do rejestru rax 
    add     rax, r11                    ; dodajê do wartoœci rejestru rax wartoœæ r11, by byæ w odpowiednim miejscu w obrazie wejœciowym
                                        ; w rax jestem w odpowiednim miejscu obrazu wejœciowego
    mov     edx, indexOut               ; przepisujê indeks w tablicy wartoœci sk³adowej czerwonej piksela obrazu wyjœciowego do rejestru edx
    movsxd  r11, edx                    ; przepisujê wartoœæ rejestru edx do rejestru rcx (movsxd, poniewa¿ edx jest 32bitowy, a rcx 64 bitowy)
    lea     r10, [r11+2]                ; wczytuje efektywny adres r11+2, czyli wartoœæ sk³adowej niebieskiej obrazu wejœciowego, do r10
    mov     r11, QWORD PTR [rcx+24]     ; przepisujê wartoœæ [rcx+24] (adres pierwszego elementu obrazu wyjœciowego) do rejestru r11
    add     r11, r10                    ; dodajê do wartoœci rejestru r11 wartoœæ r10, by byæ w odpowiednim miejscu w obrazie wyjœciowym
                                        ; w r11 jestem w odpowiednim miejscu obrazu wyjœciowego
    movzx   eax, BYTE PTR [rax]         ; przepisujê wartoœæ z adresu [rax], czyli wartoœæ odpowiedniego piksela w obrazie wejœciowym, do rejestru eax
    mov     BYTE PTR [r11], al          ; przepisujê rejestr al (8 bitów z rejestru eax) do wartoœci z odpowiedniego piksela w obrazie wyjœciowym
    ; w tej iteracji wartoœci pikseli w obrazie wyjœciowym zosta³y zamienione

    add     innerIterator, 1        ; do iteratora pêtli wewnêtrznej dodajê 1
    jmp     INNER_LOOP              ; skacze bezwarunkowo do pocz¹tku pêtli wewnêtrznej

INNER_LOOP_END:
    add     outerIterator, 1        ; do iteratora pêtli zewnêtrznej dodajê 1
    jmp     OUTER_LOOP              ; skacze bezwarunkowo do pocz¹tku pêtli zewnêtrznej

OUTER_LOOP_END:

    pop     rbp         ; zdejmuje ze szczytu stosu wartoœæ i przepisujê j¹ do rbp
    ret                 ; powrót z procedury

nearestNeighbourAsm endp 
end