# SimonSays Laguntza Gida

## Ikuspegi orokorra
SimonSays Windows-eko testutik hizketara (TTS) laguntzailea da, Juan Rey Saurak sortua, hizketa azkarra, esaldi-liburutegiak eta lerro barruko soinuen erreprodukziorako optimizatua. Hizkuntza anitz, ahots konfiguragarriak eta erabiltzaileentzako funtzio eraginkorrak onartzen ditu.

SimonSays-ek Windows ataza-barraren ezkerreko eremu librea erabiltzen du, hamaika kategoriako maiz erabiltzen diren (eta pertsonalizagarriak diren) esaldietara sarbide azkarra emanez klik pare batekin.

## Laster-teklak
- `F1`: Laguntza ireki.
- `F2`: Ezarpenak ireki.
- `F3`: Uneko hautapenaren ondoren gehitu (kategoria/esaldi testuingurua).
- `F4`: Uneko hautapena editatu.
- `F5` / `F6`: Aurreko / hurrengo hautapenera mugitu zerrendetan.
- `F8`: Uneko hautapena ezabatu.
- `F9`: Kategoriak inportatu.
- `F10`: Kategoriak esportatu.
- `Erreproduzitu>` botoia: Erreprodukzioa hasi.

## Abiaraztea eta oinarrizko erabilera
1. Abiarazi SimonSays; Windows-eko ataza-barrako erretiluan agertzen da.
2. Hitz egiteko, honako hauek egin ditzakezu:
   - Testu pertsonalizatua idatzi sarrera-koadroan eta `Erreproduzitu>` botoia sakatu (edo `Enter`).
   - Gordetako esaldi bat edo gehiago klikatu Kategorien leihoan.
3. Erreprodukzioan, botoiak `>` erakusten du erreprodukzio aktiboa adierazteko. Berriro saka dezakezu berehala gelditzeko.

## Kategorien leihoa (esaldiak)
- Ireki `Kategoriak` botoiaren bidez. Tamaina aldatu edo gogoratu Ezarpenetan.
- Hautatu kategoria bat goian eta esaldiak behean.
- Esaldi batean klik bakarrak berehala hitz egiten du `Hautatzean esaldia berehala esan` aktibatuta badago; bestela, esaldia sarrera-koadro nagusian kargatzen da hitz egin gabe.

### Kategoriak kudeatu
- Gehitu: hautatu kategoria → (laster-tekla: `F3`) `Kategoria berria gehitu` elkarrizketa → idatzi izena → `Ados`.
- Berrizendatu: hautatu kategoria → (laster-tekla: `F4`) `Kategoriaren izena aldatu` → aldatu izena → `Ados`.
- Hautapena mugitu: `F5`/`F6` aurreko/hurrengo elementura mugitzeko.
- Ezabatu: hautatu kategoria → (laster-tekla: `F8`) `Kategoria ezabatu`; berretsi kategoria eta esaldien ezabaketa.

### Esaldiak kudeatu
- Gehitu: hautatu esaldia → (laster-tekla: `F3`) `Esaldi berria gehitu` → idatzi testua → `Ados`.
- Editatu: hautatu esaldia → (laster-tekla: `F4`) `Esaldia editatu` → eguneratu testua → `Ados`.
- Hautapena mugitu: `F5`/`F6` aurreko/hurrengo esaldira mugitzeko.
- Ezabatu: hautatu esaldia → (laster-tekla: `F8`) `Esaldia ezabatu`; berretsi.

## Ezarpenak (F2)
Ireki Ezarpenak (`F2`) honako hauek kontrolatzeko:
- **Testu lehenetsia**: gaitu/desgaitu `Txertatu testu lehenetsia automatikoki` eta pertsonalizatu esaldi lehenetsia.
  - `Ukipen teklatua erakutsi` — pantailako ukipen teklatua irekitzen du sarrera-koadroak fokua jasotzen duenean, leiho nagusitik gertu kokatuta.
- **Interfaze-hizkuntza**: UI hizkuntza aukeratu (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Ahotsa**: instalatutako SAPI ahotsa hautatu zure hobespenen arabera.
- **Bolumena/Abiadura**: doitu `Bolumena (10-100)` eta `Abiadura (-10etik 10era)`.
- `Ahotsa probatu`: hautatutako ahotsa probatu.
- **Kategoriaren leihoa**:
  - `Minimizatu kategoriaren leihoa automatikoki` hautatu ondoren.
  - `Gogoratu kategoriaren leihoaren tamaina`.
- **Erreprodukzio-portaera**:
  - `Hautatzean esaldia berehala esan` (klik bidezko auto-erreprodukzioa).
  - `Aurreko erreprodukzioa gelditu` — automatikoki gelditzen du abian dagoen edozein erreprodukzio esaldi berri bat hasi aurretik.
  - `Handitu SimonSays bolumena erreproduzitzean` — sistemaren bolumen nagusia igotzen du hitz egiten den bitartean, erreprodukzioa amaitzean leheneratuz.
  - `Murriztu aldi baterako beste audioak erreproduzitzean` — beste aplikazioen bolumena murrizten du hitz egiten den bitartean, erreprodukzioa amaitzean leheneratuz.
- `Ados` aldaketak gordetzen ditu; `Utzi` baztertzen ditu.

## Erretiluko ikonoaren menua
- `Erakutsi` / `Ezkutatu` interfaze nagusia.
- `Ezarpenak`.
- `Honi buruz` bertsioa eta kredituak erakusten ditu.
- `Webgunea` proiektuaren orria irekitzen du (konfiguratuta badago).
- `Feedback` iritzi-formulario bat irekitzen du nabigatzaile lehenetsian.
- `Irten` SimonSays ixten du.

Erretiluko leiho gainerakorra eta beste elkarrizketak mugitu ditzakezu titulu-barra arrastatuz; Kategorien leihoaren tamaina aldatu ertzak arrastatuz, eta gaitu `Gogoratu kategoriaren leihoaren tamaina` Ezarpenetan tamaina gordetzeko.

## Hizketa eta soinuak nahastu
- Erabili `♫` (`SOUND_NOTE_DELIMITER`) mugatzailea testuan audio fitxategien izenak txertatzeko:
  - Adibidea: `Kaixo ♫notification.wav♫, itxaron mesedez.`
  - Mugatzaileen kanpoko testua hitz egiten da; barrualdekoa soinu-fitxategi gisa tratatzen da eta lerro barruan erreproduzitzen da.
- Onartutako audioa: `.wav`, `.mid`, `.midi`, `.mp3`.
- Erabili bide absolutuak edo erlatiboak prozesuarentzat eskuragarriak. Parekatu mugatzaileak amaierako testua audio gisa tratatzeari eusteko.

## Kategoriak inportatu eta esportatu
- Esportatu: kategoria guztiak edo hautatutakoa bakarrik esporta ditzakezu galdetzen zaizunean. Arrakasta/hutsegiteen mezuak hautatutako hizkuntzan agertzen dira (laster-tekla: `F10`).
- Inportatu: aukeratu fitxategi bat; kategoria existitzen bada, gainidaztea eskatuko zaizu. Arrakasta/hutsegiteen mezuak hautatutako hizkuntzan agertzen dira (laster-tekla: `F9`).

## Arazoen konponketa
- **Ahotsaren hasieratzeak huts egin du**: egiaztatu SAPI instalatuta dagoela eta ahots bateragarria dagoela.
- **Soinurik ez da erreproduzitzen**: berretsi fitxategiaren bidea/luzapena eta `♫` mugatzaile bikoitzak.
- **Inportazio/esportazio erroreak**: egiaztatu baimenak eta diskoaren kokalekua; saiatu berriro.
- **UI hizkuntza ez da aplikatu**: berrireki Ezarpenak eta berretsi hizkuntza-hautaketa; ziurtatu lokalizazio-kateak existitzen direla hautatutako hizkuntzarako.

## Honi buruz
- `Honi buruz` elkarrizketa-koadroak bertsioa, deskribapena eta copyright-a erakusten ditu.
