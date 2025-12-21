#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* ssid = "Zerzevan_Kalesi";
const byte DNS_PORT = 53;

WebServer server(80);
DNSServer dnsServer;

// ========== BAĞLAM HAFIZASI ==========
struct BaglamHafiza {
  String sonKonu;           // Son konuşulan konu
  String bekleyenKonu;      // "Evet" bekleyen konu
  String sonSoru;           // Son sorulan soru
  String sonCevap;          // Son verilen cevap
  int konusmaSayisi;        // Toplam konuşma sayısı
  bool takipSorusuVar;      // Takip sorusu soruldu mu?
  unsigned long sonZaman;   // Son mesaj zamanı
};

BaglamHafiza hafiza = {"", "", "", "", 0, false, 0};

// ========== KONU YAPISI ==========
struct Konu {
  String id;
  String anahtar[20];
  int agirlik[20];
  int anahtarSayisi;
  String cevaplar[8];
  int cevapSayisi;
  String takipSorulari[4];
  int takipSayisi;
};

// ========== ONAY KELİMELERİ ==========
String onayKelimeleri[] = {"evet", "yes", "olur", "tamam", "ok", "okay", "istiyorum", 
                           "isterim", "tabii", "tabi", "sure", "peki", "anlat", 
                           "söyle", "soyle", "devam", "lutfen", "lütfen"};
int onayKelimeSayisi = 18;

String redKelimeleri[] = {"hayır", "hayir", "no", "istemiyorum", "istemem", "yok", "gerek yok"};
int redKelimeSayisi = 7;

// ========== TÜRKÇE KONULAR ==========
Konu konular[] = {
  // 0 - SELAMLAMA
  {"selamlama",
    {"merhaba", "selam", "hey", "hello", "hi", "günaydın", "iyi günler", "naber", "nasılsın", "hosgeldin"},
    {100, 100, 90, 100, 90, 95, 95, 80, 70, 85},
    10,
    {
      "Merhaba! 🏰 Ben Zerzevan Kalesi'nin dijital rehberiyim. Size bu 1700 yıllık Roma kalesi hakkında her şeyi anlatabilirim!",
      "Selam! Hoş geldiniz! Antik Roma'nın bu muhteşem savunma yapısını keşfetmeye hazır mısınız?",
      "Merhaba değerli ziyaretçi! 🏛️ Zerzevan'ın büyüleyici tarihini sizinle paylaşmak için sabırsızlanıyorum!",
      "Hey! Roma İmparatorluğu'nun doğu sınırındaki bu efsanevi kaleye hoş geldiniz!"
    }, 4,
    {"Tarihi mi, mimarisi mi, yoksa gizemli Mithra Tapınağı mı ilginizi çekiyor?", 
     "Hangi konuda bilgi almak istersiniz? Tarih, mimari, askeri yapı?",
     "Size önce kalenin tarihini mi anlatsam?"}, 3
  },
  
  // 1 - TARİH
  {"tarih",
    {"tarih", "tarihi", "history", "ne zaman", "yapıldı", "yapildi", "inşa", "insa", "kaç yıl", "eski", "antik", "geçmiş", "kuruluş", "kurulus"},
    {80, 85, 80, 70, 75, 75, 70, 70, 65, 50, 60, 55, 70, 70},
    14,
    {
      "Zerzevan Kalesi, Milattan Sonra 3. yüzyılda Roma İmparatorluğu tarafından inşa edildi. Tam 1700 yıllık devasa bir tarih! 🏛️ Sasani İmparatorluğu'na karşı doğu sınırını korumak için stratejik bir nokta olarak kuruldu.",
      "Kalenin temelleri MS 250-300 yılları arasında atıldı. İmparator Diocletianus döneminde, Sasanilere karşı savunma hattının kritik bir parçası haline geldi. O dönem için inanılmaz gelişmiş bir askeri kompleks!",
      "3. yüzyıl Roma mühendisliğinin şaheseri! Kale, Roma'nın doğu sınırlarını koruyan 'Limes Orientalis' savunma hattının en önemli noktalarından biriydi. Yaklaşık 400 yıl aktif olarak kullanıldı.",
      "Antik çağın en stratejik sınır kalelerinden biri! İpek Yolu üzerindeki konumu sayesinde hem ticaret hem askeri açıdan muazzam öneme sahipti. Bizans döneminde de kullanılmaya devam etti."
    }, 4,
    {"Kalenin mimarisini de öğrenmek ister misiniz?", 
     "Mithra Tapınağı hakkında bilgi vermemi ister misiniz?",
     "Askeri yapısını da anlatayım mı?"}, 3
  },
  
  // 2 - MİTHRA
  {"mithra",
    {"mithra", "mitra", "tapınak", "tapinak", "temple", "din", "tanrı", "tanri", "ibadet", "gizem", "tören", "toren", "yeraltı", "yeralti", "gizli"},
    {100, 100, 85, 85, 80, 60, 55, 55, 65, 70, 65, 65, 70, 70, 60},
    15,
    {
      "Mithra Tapınağı kalenin en gizemli bölümü! 🔮 Roma askerleri burada güneş tanrısı Mithra'ya gizli törenler düzenlerdi. Anadolu'daki en iyi korunmuş Mithraeum'lardan biri. Yeraltı tapınağında karanlıktan aydınlığa geçiş ritüelleri yapılırdı.",
      "Mithraizm, o dönemde Roma ordusunda son derece popüler bir gizem diniydi. Tapınak kasıtlı olarak yeraltına inşa edilmişti - sadece inisiye olmuş askerler girebilirdi! Kozmik güçleri temsil eden sembollerle doluydu.",
      "Bu tapınak 7 farklı inisiyasyon seviyesi olan gizli bir dinsel mekandı. Askerler burada cesaret, sadakat ve fedakarlık değerlerini pekiştirirdi. Boğa kurban sahneleriyle süslü duvarları hala görülebiliyor!",
      "12 metre uzunluğundaki yeraltı odası, orijinal sunağı ve ritüel platformlarıyla arkeolojik bir hazine! Roma'nın askeri dinsel yaşamını anlamamız için benzersiz bir pencere açıyor."
    }, 4,
    {"Tapınaktaki 7 inisiyasyon seviyesini detaylı anlatmamı ister misiniz?",
     "Kalenin mimarisi hakkında da bilgi vermemi ister misiniz?",
     "Roma askeri yaşamını merak ediyor musunuz?"}, 3
  },
  
  // 3 - MİMARİ
  {"mimari",
    {"mimari", "architecture", "yapı", "yapi", "bina", "duvar", "kule", "sur", "taş", "tas", "büyüklük", "buyukluk", "nasıl yapılmış", "inşaat", "insaat", "mühendislik"},
    {90, 85, 70, 70, 60, 75, 75, 70, 60, 60, 55, 55, 80, 65, 65, 70},
    16,
    {
      "Zerzevan Kalesi 12 hektar alanı kaplayan devasa bir kompleks! 🏗️ İki sıra halinde 12-15 metre yüksekliğinde kalın surları, 12 gözetleme kulesi, su sarnıçları ve yeraltı tünelleri var. Roma mühendislik dehasının canlı kanıtı!",
      "Bazalt taşlarından örülmüş duvarlar tam 3 metre kalınlığa ulaşıyor! Stratejik tepe konumu sayesinde düşman Dicle Vadisi'nden kilometrelerce uzaktan görülebiliyordu. Savunma sistemi dönemine göre son derece gelişmiş.",
      "Üç ana bölümden oluşuyor: askeri kışlalar, Mithra tapınağı ve idari binalar. Su sarnıçları kuşatmalarda aylar boyu dayanabilecek kapasitedeydi. Yeraltı tünelleri gizli kaçış yolları sağlıyordu!",
      "Ortogonal planlama sistemi kullanılmış - tipik Roma askeri mimarisi! Ana kapı, köprü ve hendek sistemi düşmanı yavaşlatacak şekilde tasarlanmış. Depreme dayanıklı teknikler sayesinde 1700 yıl sonra hala ayakta!"
    }, 4,
    {"Su sistemini detaylı anlatmamı ister misiniz?",
     "Askeri yapısı hakkında bilgi vermemi ister misiniz?",
     "Kalenin konumunu merak ediyor musunuz?"}, 3
  },
  
  // 4 - ASKERİ
  {"askeri",
    {"asker", "askeri", "military", "ordu", "army", "lejyon", "savunma", "savaş", "savas", "garnizon", "komutan", "silah", "strateji", "nöbet", "nobet"},
    {85, 90, 85, 75, 75, 80, 80, 70, 70, 85, 65, 60, 70, 55, 55},
    15,
    {
      "Zerzavan tam teşekküllü bir askeri garnizondu! ⚔️ Yaklaşık 300 Roma lejyoneri sürekli burada konuşlanırdı. Sasani İmparatorluğu'na karşı erken uyarı sistemi ve ilk savunma hattıydı.",
      "Kale Roma'nın Doğu Sınır Komutanlığı'nın (Dux Orientis) kontrolündeydi. Kumandan, subaylar, lejyonerler ve yardımcı birlikler burada görev yapardı. Askeri hiyerarşi ve disiplin son derece katıydı!",
      "Stratejik konumu sayesinde hem savunma hem saldırı üssü olarak kullanılıyordu. Bölgedeki Roma ordusuna lojistik destek sağlar, İpek Yolu güvenliğini kontrol ederdi.",
      "Roma lejyonerlerinin yaşamı burada çok düzenliydi: sabah eğitimleri, nöbet, silah bakımı, inşaat... Mithra tapınağı askerler için manevi bir sığınak, moral kaynağıydı."
    }, 4,
    {"Roma-Sasani savaşları hakkında bilgi vermemi ister misiniz?",
     "Askerlerin günlük yaşamını anlatayım mı?",
     "Kalenin tarihini de öğrenmek ister misiniz?"}, 3
  },
  
  // 5 - KONUM
  {"konum",
    {"nerede", "where", "konum", "location", "yer", "adres", "nasıl gidilir", "nasil gidilir", "ulaşım", "ulasim", "yol", "diyarbakır", "diyarbakir", "çınar", "cinar"},
    {95, 90, 90, 85, 60, 70, 85, 85, 75, 75, 55, 70, 70, 75, 75},
    15,
    {
      "Zerzavan Kalesi Diyarbakır ili Çınar ilçesinde! 📍 Şehir merkezine sadece 13 km mesafede. Demirölçek köyü yakınlarında, Dicle Vadisi'ne hakim stratejik bir tepe üzerinde konumlanmış.",
      "Ulaşım çok kolay! Diyarbakır'dan arabayla yaklaşık 20 dakika. D-950 karayolu üzerinden Çınar istikametine gidip tabelalara uyabilirsiniz. GPS'e 'Zerzavan Kalesi' yazmanız yeterli.",
      "Coğrafi koordinatlar: 37°43'K, 40°26'D. Deniz seviyesinden 660 metre yükseklikte. Dicle Nehri'nin 5 km doğusunda - stratejik konum için mükemmel seçilmiş!",
      "Kale İpek Yolu'nun önemli bir kavşağındaydı. Bu yüzden hem ticari hem askeri açıdan kritik öneme sahipti. Antik çağda yol ağlarının merkezi!"
    }, 4,
    {"Ziyaret saatleri hakkında bilgi vermemi ister misiniz?",
     "Günümüzdeki durumunu anlatayım mı?",
     "Yakındaki diğer tarihi yerler hakkında bilgi ister misiniz?"}, 3
  },
  
  // 6 - GÜNÜMÜZ
  {"gunumuz",
    {"günümüz", "gunumuz", "bugün", "bugun", "şimdi", "simdi", "today", "now", "ziyaret", "visit", "turizm", "müze", "muze", "unesco", "restorasyon", "kazı", "kazi"},
    {85, 85, 70, 70, 65, 65, 75, 70, 80, 75, 75, 70, 70, 90, 75, 85, 85},
    17,
    {
      "Günümüzde Zerzavan UNESCO Dünya Mirası Geçici Listesi'nde! 🌍 Her yıl binlerce yerli ve yabancı turist ziyaret ediyor. Kültür ve Turizm Bakanlığı tarafından korunuyor. Üstelik giriş ücretsiz!",
      "Aktif arkeolojik kazılar 2014'ten beri devam ediyor. Mardin Müzesi ekipleri her yıl yeni keşifler yapıyor! Restorasyon çalışmaları dikkatli ve bilimsel yöntemlerle sürdürülüyor.",
      "Turizm için harika bir destinasyon! Dicle Vadisi manzarası eşsiz güzellikte. Özellikle gün batımında fotoğraf çekmek için muhteşem. Yakında modern ziyaretçi merkezi açılması planlanıyor.",
      "2020'de UNESCO Dünya Mirası Geçici Listesi'ne alındı - büyük bir başarı! Kalıcı listeye alınması için çalışmalar sürüyor. Türkiye'nin en önemli arkeolojik alanlarından biri!"
    }, 4,
    {"Kazılarda bulunan eserler hakkında bilgi vermemi ister misiniz?",
     "Ziyaret için pratik bilgiler ister misiniz?",
     "Kalenin tarihini de öğrenmek ister misiniz?"}, 3
  },
  
  // 7 - ROMA İMPARATORLUĞU
  {"roma",
    {"roma", "roman", "rome", "imparatorluk", "empire", "latin", "caesar", "sezar", "diocletianus", "bizans", "byzantine"},
    {85, 80, 75, 70, 65, 55, 60, 60, 75, 70, 65},
    11,
    {
      "Roma İmparatorluğu döneminde bu bölge hayati öneme sahipti! Mezopotamya'ya açılan kapıydı. Sasani İmparatorluğu ile sürekli rekabet vardı ve Zerzavan bu rekabetin tam ön cephesindeydi.",
      "İmparator Diocletianus döneminde doğu sınırları yeniden organize edildi. Zerzavan bu yeni savunma planının kilit taşlarından biriydi. Tetrarchy (dörtlü yönetim) sistemi döneminde stratejik önemi arttı.",
      "Romalılar burada sadece askerlik yapmadı, Roma kültürünü de yaydı. Latin dili, Roma hukuku, mimari, din... Hepsi Anadolu'ya Zerzavan gibi noktalardan yayıldı.",
      "Roma lejyonerleri burada yıllarca görev yaptı. Anadolu, Balkanlar, hatta Britanya'dan gelen askerler! Gerçek anlamda kozmopolit bir askeri topluluktu."
    }, 4,
    {"Sasani İmparatorluğu hakkında bilgi vermemi ister misiniz?",
     "Roma askeri yaşamını anlatayım mı?",
     "Kalenin mimarisini merak ediyor musunuz?"}, 3
  },
  
  // 8 - SASANİ
  {"sasani",
    {"sasani", "sassanian", "pers", "persian", "iran", "düşman", "dusman", "rakip", "savaş", "savas", "doğu", "dogu"},
    {100, 95, 80, 75, 70, 65, 65, 60, 55, 55, 50, 50},
    12,
    {
      "Sasani İmparatorluğu, Roma'nın doğudaki en büyük ve en tehlikeli rakibiydi! ⚔️ Pers kültürünün varisi olan Sasaniler, güçlü orduları ve gelişmiş medeniyetleriyle Roma'ya sürekli meydan okuyordu.",
      "Roma-Sasani savaşları yüzyıllarca sürdü. Zerzavan gibi sınır kaleleri bu çatışmaların tam merkezindeydi. Bazen barış anlaşmaları yapılsa da gerginlik hiç bitmedi.",
      "Sasaniler İran platosu ve Mezopotamya'nın büyük bölümünü kontrol ediyordu. İpek Yolu'nun önemli kısmı onların elindeydi. Roma ile hem askeri hem ticari yoğun rekabet içindeydiler.",
      "İlginç olan şu: Roma ve Sasani kültürleri birbirini derinden etkiledi! Mimari teknikler, askeri stratejiler, hatta dinler bile karıştı. Zerzavan bu kültürel etkileşimin canlı tanığı."
    }, 4,
    {"Roma İmparatorluğu hakkında daha fazla bilgi ister misiniz?",
     "Kalenin askeri yapısını anlatayım mı?",
     "Tarihini baştan anlatmamı ister misiniz?"}, 3
  },
  
  // 9 - SU SİSTEMİ
  {"su",
    {"su", "water", "sarnıç", "sarnic", "kuyu", "well", "cistern", "kaynak", "içme", "icme", "depo"},
    {75, 70, 90, 90, 80, 75, 85, 65, 55, 55, 70},
    11,
    {
      "Kalenin su sistemi mühendislik harikası! 💧 Yağmur suyu toplama sistemleri, dev sarnıçlar ve dağıtım kanalları son derece gelişmişti. 300 asker ve aileleri için aylarca yetecek su depolanabiliyordu.",
      "Sarnıçlar stratejik olarak yeraltına inşa edilmişti - hem serin tutmak hem de düşmandan korumak için. Kuşatma durumunda su en kritik kaynak olduğu için özel önem verilmişti.",
      "Roma mühendisleri su yönetiminde ustaydı! Zerzavan'da bunu açıkça görüyoruz. Kanallar, filtreler ve depolama alanları entegre bir sistem oluşturuyordu."
    }, 3,
    {"Kalenin mimarisi hakkında daha fazla bilgi ister misiniz?",
     "Askeri yaşamı anlatayım mı?"}, 2
  },
  
  // 10 - YARDIM
  {"yardim",
    {"yardım", "yardim", "help", "ne sorabilirim", "neler var", "özellik", "ozellik", "komut", "nasıl kullanılır", "ne yapabilirsin"},
    {100, 100, 95, 90, 85, 70, 70, 75, 85, 90},
    10,
    {
      "Size yardımcı olmaktan mutluluk duyarım! 📚 Şunları sorabilirsiniz:\n\n🏛️ Tarih: 'Ne zaman yapıldı?', 'Tarihi nedir?'\n🔮 Mithra: 'Tapınak nedir?', 'Mithra kimdir?'\n🏗️ Mimari: 'Nasıl yapılmış?', 'Kaç kule var?'\n⚔️ Askeri: 'Kaç asker vardı?', 'Savunma sistemi?'\n📍 Konum: 'Nerede?', 'Nasıl gidilir?'\n🌍 Günümüz: 'Ziyaret edilebilir mi?', 'Kazılar?'",
      "Ben Zerzavan Kalesi hakkında her konuda bilgi verebilirim! Tarih, mimari, Mithra tapınağı, askeri yapı, konum, günümüzdeki durum... İstediğinizi sorun!"
    }, 2,
    {"Nereden başlamak istersiniz? Tarihi mi anlatsam?"}, 1
  }
};

int konuSayisi = 11;

// ========== DİL ALGILAMA ==========
bool isTurkish(String s) {
  s.toLowerCase();
  if(s.indexOf("ç")!=-1 || s.indexOf("ğ")!=-1 || s.indexOf("ı")!=-1 ||
     s.indexOf("ö")!=-1 || s.indexOf("ş")!=-1 || s.indexOf("ü")!=-1) return true;
  
  int tr = 0, en = 0;
  String trK[] = {"nedir", "nerede", "nasıl", "anlat", "hakkında", "evet", "hayır"};
  String enK[] = {"what", "where", "how", "tell", "about", "yes", "no"};
  
  for(int i=0; i<7; i++) {
    if(s.indexOf(trK[i]) != -1) tr++;
    if(s.indexOf(enK[i]) != -1) en++;
  }
  return tr >= en;
}

// ========== ONAY/RED KONTROLÜ ==========
bool onayMi(String soru) {
  soru.toLowerCase();
  for(int i = 0; i < onayKelimeSayisi; i++) {
    if(soru.indexOf(onayKelimeleri[i]) != -1) return true;
  }
  return false;
}

bool redMi(String soru) {
  soru.toLowerCase();
  for(int i = 0; i < redKelimeSayisi; i++) {
    if(soru.indexOf(redKelimeleri[i]) != -1) return true;
  }
  return false;
}

// ========== KELİME ARAMA ==========
bool kelimeVar(String cumle, String kelime) {
  cumle.toLowerCase();
  kelime.toLowerCase();
  return cumle.indexOf(kelime) != -1;
}

// ========== KONU BULMA ==========
int konuBul(String soru) {
  soru.toLowerCase();
  int enYuksekPuan = 0;
  int enIyiKonu = -1;
  
  for(int k = 0; k < konuSayisi; k++) {
    int toplamPuan = 0;
    for(int a = 0; a < konular[k].anahtarSayisi; a++) {
      if(kelimeVar(soru, konular[k].anahtar[a])) {
        toplamPuan += konular[k].agirlik[a];
      }
    }
    if(toplamPuan > enYuksekPuan) {
      enYuksekPuan = toplamPuan;
      enIyiKonu = k;
    }
  }
  
  return (enYuksekPuan >= 50) ? enIyiKonu : -1;
}

// ========== BAĞLAMSAL GİRİŞ ==========
String baglamGiris(String soru, int konuIndex) {
  String girisler[6];
  int girisS = 0;
  
  switch(konuIndex) {
    case 1: // Tarih
      if(kelimeVar(soru, "ne zaman") || kelimeVar(soru, "when")) {
        girisler[girisS++] = "Harika bir soru! ";
        girisler[girisS++] = "Çok merak edilen bir konu! ";
      } else {
        girisler[girisS++] = "Tarihi gerçekten çok ilginç! ";
        girisler[girisS++] = "Geçmişe yolculuk yapalım: ";
      }
      break;
    case 2: // Mithra
      girisler[girisS++] = "Ah, Mithra Tapınağı! Benim favorim. ";
      girisler[girisS++] = "Çok gizemli bir konu! ";
      girisler[girisS++] = "En ilginç bölüm burası! ";
      break;
    case 3: // Mimari
      girisler[girisS++] = "Mimarisi gerçekten etkileyici! ";
      girisler[girisS++] = "Roma mühendisleri harika iş çıkarmış. ";
      break;
    case 4: // Askeri
      girisler[girisS++] = "Askeri yapısı çok önemli! ";
      girisler[girisS++] = "Stratejik açıdan kritik bir nokta! ";
      break;
    case 5: // Konum
      girisler[girisS++] = "Konumu çok stratejik seçilmiş! ";
      girisler[girisS++] = "Ulaşımı aslında çok kolay. ";
      break;
    case 6: // Günümüz
      girisler[girisS++] = "Günümüzde çok popüler bir yer! ";
      girisler[girisS++] = "Ziyaret etmeye değer! ";
      break;
    default:
      return "";
  }
  
  if(girisS == 0) return "";
  return girisler[random(0, girisS)];
}

// ========== KULLANICI REFERANSI ==========
String kullaniciReferansi(String soru) {
  soru.toLowerCase();
  
  if(kelimeVar(soru, "mithra") || kelimeVar(soru, "mitra") || kelimeVar(soru, "tapınak") || kelimeVar(soru, "tapinak")) 
    return "Mithra Tapınağı";
  if(kelimeVar(soru, "tarih")) return "tarih";
  if(kelimeVar(soru, "mimari") || kelimeVar(soru, "yapı") || kelimeVar(soru, "yapi")) return "mimari";
  if(kelimeVar(soru, "asker") || kelimeVar(soru, "ordu")) return "askeri yapı";
  if(kelimeVar(soru, "nerede") || kelimeVar(soru, "konum")) return "konum";
  if(kelimeVar(soru, "su") || kelimeVar(soru, "sarnıç") || kelimeVar(soru, "sarnic")) return "su sistemi";
  
  return "";
}

// ========== ANA CEVAP FONKSİYONU ==========
String getCevap(String soru) {
  soru.trim();
  String orijinalSoru = soru;
  
  // ===== ONAY KONTROLÜ - ÖNCEKİ KONUYA DEVAM =====
  if(onayMi(soru) && hafiza.takipSorusuVar && hafiza.bekleyenKonu != "") {
    // Kullanıcı "evet" dedi ve bekleyen konu var!
    int konuIdx = -1;
    for(int i = 0; i < konuSayisi; i++) {
      if(konular[i].id == hafiza.bekleyenKonu) {
        konuIdx = i;
        break;
      }
    }
    
    if(konuIdx != -1) {
      String cevap = "Tabii ki! 😊 ";
      cevap += konular[konuIdx].cevaplar[random(0, konular[konuIdx].cevapSayisi)];
      
      // Yeni takip sorusu
      if(konular[konuIdx].takipSayisi > 0 && random(0, 100) > 40) {
        // Farklı bir konu öner
        int yeniKonu = (konuIdx + 1 + random(0, 3)) % konuSayisi;
        if(yeniKonu == 0) yeniKonu = 1;
        hafiza.bekleyenKonu = konular[yeniKonu].id;
        cevap += "\n\n" + konular[konuIdx].takipSorulari[random(0, konular[konuIdx].takipSayisi)];
        hafiza.takipSorusuVar = true;
      } else {
        hafiza.takipSorusuVar = false;
        hafiza.bekleyenKonu = "";
      }
      
      hafiza.sonKonu = konular[konuIdx].id;
      hafiza.sonCevap = cevap;
      hafiza.konusmaSayisi++;
      return cevap;
    }
  }
  
  // ===== RED KONTROLÜ =====
  if(redMi(soru) && hafiza.takipSorusuVar) {
    hafiza.takipSorusuVar = false;
    hafiza.bekleyenKonu = "";
    
    String redCevaplari[] = {
      "Tamam, başka bir konuda yardımcı olabilir miyim? 😊",
      "Anladım! Başka ne merak ediyorsunuz?",
      "Peki, farklı bir şey sormak ister misiniz?",
      "Sorun değil! Başka bir konuda bilgi verebilirim."
    };
    return redCevaplari[random(0, 4)];
  }
  
  // ===== NORMAL KONU ARAMA =====
  int konuIdx = konuBul(soru);
  
  if(konuIdx == -1) {
    // Konu bulunamadı
    if(hafiza.sonKonu != "" && hafiza.konusmaSayisi > 0) {
      return "Hmm, tam anlayamadım. 🤔 " + hafiza.sonKonu + " hakkında konuşuyorduk. Başka bir konu mu merak ediyorsunuz? Tarih, mimari, Mithra tapınağı, askeri yapı veya konum sorabilirsiniz!";
    }
    return "Bu konuda yeterli bilgim yok. 🤔 Zerzevan Kalesi'nin tarihi, mimarisi, Mithra tapınağı, askeri yapısı veya konumu hakkında sorabilirsiniz!";
  }
  
  // ===== CEVAP OLUŞTURMA =====
  String cevap = "";
  
  // Kullanıcı referansı
  String ref = kullaniciReferansi(orijinalSoru);
  if(ref != "" && konuIdx != 0) {
    cevap += "'" + ref + "' hakkında soruyorsunuz, ";
  }
  
  // Bağlamsal giriş
  cevap += baglamGiris(orijinalSoru, konuIdx);
  
  // Ana cevap
  cevap += konular[konuIdx].cevaplar[random(0, konular[konuIdx].cevapSayisi)];
  
  // Takip sorusu
  if(konular[konuIdx].takipSayisi > 0 && random(0, 100) > 30) {
    int takipIdx = random(0, konular[konuIdx].takipSayisi);
    cevap += "\n\n" + konular[konuIdx].takipSorulari[takipIdx];
    hafiza.takipSorusuVar = true;
    
    // Bekleyen konuyu belirle
    String takipSorusu = konular[konuIdx].takipSorulari[takipIdx];
    takipSorusu.toLowerCase();
    
    if(kelimeVar(takipSorusu, "mimari")) hafiza.bekleyenKonu = "mimari";
    else if(kelimeVar(takipSorusu, "tarih")) hafiza.bekleyenKonu = "tarih";
    else if(kelimeVar(takipSorusu, "mithra") || kelimeVar(takipSorusu, "tapınak")) hafiza.bekleyenKonu = "mithra";
    else if(kelimeVar(takipSorusu, "asker") || kelimeVar(takipSorusu, "ordu")) hafiza.bekleyenKonu = "askeri";
    else if(kelimeVar(takipSorusu, "konum") || kelimeVar(takipSorusu, "nerede")) hafiza.bekleyenKonu = "konum";
    else if(kelimeVar(takipSorusu, "günümüz") || kelimeVar(takipSorusu, "ziyaret")) hafiza.bekleyenKonu = "gunumuz";
    else if(kelimeVar(takipSorusu, "sasani")) hafiza.bekleyenKonu = "sasani";
    else if(kelimeVar(takipSorusu, "roma")) hafiza.bekleyenKonu = "roma";
    else if(kelimeVar(takipSorusu, "su") || kelimeVar(takipSorusu, "sarnıç")) hafiza.bekleyenKonu = "su";
    else hafiza.bekleyenKonu = konular[(konuIdx + 1) % konuSayisi].id;
  } else {
    hafiza.takipSorusuVar = false;
  }
  
  // Hafızayı güncelle
  hafiza.sonKonu = konular[konuIdx].id;
  hafiza.sonSoru = orijinalSoru;
  hafiza.sonCevap = cevap;
  hafiza.konusmaSayisi++;
  hafiza.sonZaman = millis();
  
  return cevap;
}

// ========== CAPTIVE PORTAL HTML ==========
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Zerzavan Kalesi - AI Rehber</title>
<style>
* { margin: 0; padding: 0; box-sizing: border-box; }
body {
  font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
  min-height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 10px;
}
.container {
  background: rgba(255,255,255,0.95);
  border-radius: 24px;
  box-shadow: 0 25px 80px rgba(0,0,0,0.5);
  max-width: 500px;
  width: 100%;
  overflow: hidden;
  animation: slideUp 0.6s ease-out;
}
@keyframes slideUp {
  from { opacity: 0; transform: translateY(30px); }
  to { opacity: 1; transform: translateY(0); }
}
.header {
  background: linear-gradient(135deg, #e94560 0%, #0f3460 100%);
  color: white;
  padding: 30px 20px;
  text-align: center;
  position: relative;
  overflow: hidden;
}
.header::before {
  content: '';
  position: absolute;
  top: -50%;
  left: -50%;
  width: 200%;
  height: 200%;
  background: radial-gradient(circle, rgba(255,255,255,0.1) 0%, transparent 60%);
  animation: shimmer 3s infinite;
}
@keyframes shimmer {
  0%, 100% { transform: rotate(0deg); }
  50% { transform: rotate(180deg); }
}
.header h1 {
  font-size: 1.8em;
  margin-bottom: 8px;
  position: relative;
  text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
}
.header p {
  font-size: 1em;
  opacity: 0.9;
  position: relative;
}
.status {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  background: rgba(255,255,255,0.2);
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 0.85em;
  margin-top: 12px;
}
.status-dot {
  width: 8px;
  height: 8px;
  background: #00ff88;
  border-radius: 50%;
  animation: pulse 1.5s infinite;
}
@keyframes pulse {
  0%, 100% { opacity: 1; transform: scale(1); }
  50% { opacity: 0.5; transform: scale(1.2); }
}
.quick-btns {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  padding: 15px;
  background: #f8f9fa;
  justify-content: center;
}
.quick-btn {
  padding: 10px 16px;
  background: white;
  border: 2px solid #e94560;
  color: #e94560;
  border-radius: 20px;
  font-size: 13px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s;
}
.quick-btn:hover, .quick-btn:active {
  background: #e94560;
  color: white;
  transform: scale(1.05);
}
.chat {
  padding: 20px;
  max-height: 350px;
  min-height: 280px;
  overflow-y: auto;
  background: #f0f2f5;
}
.msg {
  margin-bottom: 16px;
  animation: fadeIn 0.4s ease-out;
  display: flex;
  align-items: flex-start;
  gap: 10px;
}
@keyframes fadeIn {
  from { opacity: 0; transform: translateY(10px); }
  to { opacity: 1; transform: translateY(0); }
}
.msg.user { flex-direction: row-reverse; }
.avatar {
  width: 36px;
  height: 36px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 18px;
  flex-shrink: 0;
}
.msg.bot .avatar { background: linear-gradient(135deg, #e94560, #0f3460); }
.msg.user .avatar { background: linear-gradient(135deg, #667eea, #764ba2); }
.bubble {
  padding: 14px 18px;
  border-radius: 18px;
  max-width: 80%;
  line-height: 1.6;
  font-size: 14px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.08);
  white-space: pre-line;
}
.msg.user .bubble {
  background: linear-gradient(135deg, #667eea, #764ba2);
  color: white;
  border-bottom-right-radius: 4px;
}
.msg.bot .bubble {
  background: white;
  color: #1a1a2e;
  border-bottom-left-radius: 4px;
}
.typing {
  display: none;
  padding: 14px 18px;
  background: white;
  border-radius: 18px;
  width: fit-content;
  margin-left: 46px;
}
.typing.show { display: block; }
.typing span {
  display: inline-block;
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: #e94560;
  margin: 0 2px;
  animation: bounce 1.4s infinite ease-in-out;
}
.typing span:nth-child(2) { animation-delay: 0.2s; }
.typing span:nth-child(3) { animation-delay: 0.4s; }
@keyframes bounce {
  0%, 60%, 100% { transform: translateY(0); }
  30% { transform: translateY(-8px); }
}
.input-area {
  padding: 20px;
  background: white;
  display: flex;
  gap: 10px;
  border-top: 1px solid #e9ecef;
}
input {
  flex: 1;
  padding: 14px 18px;
  border: 2px solid #e9ecef;
  border-radius: 25px;
  font-size: 15px;
  outline: none;
  transition: all 0.3s;
}
input:focus {
  border-color: #e94560;
  box-shadow: 0 0 0 3px rgba(233,69,96,0.1);
}
button {
  padding: 14px 24px;
  background: linear-gradient(135deg, #e94560, #0f3460);
  color: white;
  border: none;
  border-radius: 25px;
  font-size: 15px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s;
}
button:hover { transform: translateY(-2px); box-shadow: 0 6px 20px rgba(233,69,96,0.4); }
button:active { transform: translateY(0); }
.footer {
  background: #1a1a2e;
  color: rgba(255,255,255,0.7);
  padding: 12px;
  text-align: center;
  font-size: 12px;
}
.footer a { color: #e94560; text-decoration: none; }
</style>
</head>
<body>
<div class="container">
  <div class="header">
    <h1>🏰 Zerzavan Kalesi</h1>
    <p>Yapay Zeka Destekli Dijital Rehber</p>
    <div class="status">
      <span class="status-dot"></span>
      Çevrimdışı Çalışıyor
    </div>
  </div>
  
  <div class="quick-btns">
    <button class="quick-btn" onclick="ask('Tarihi nedir?')">📜 Tarih</button>
    <button class="quick-btn" onclick="ask('Mithra tapınağı nedir?')">🔮 Mithra</button>
    <button class="quick-btn" onclick="ask('Mimarisi nasıl?')">🏛️ Mimari</button>
    <button class="quick-btn" onclick="ask('Nerede bulunuyor?')">📍 Konum</button>
    <button class="quick-btn" onclick="ask('Günümüzde ziyaret edilebilir mi?')">🎫 Ziyaret</button>
  </div>
  
  <div class="chat" id="chat">
    <div class="msg bot">
      <div class="avatar">🏛️</div>
      <div class="bubble">Hoş geldiniz! 🏰 Ben Zerzavan Kalesi'nin dijital rehberiyim. Bu 1700 yıllık Roma kalesi hakkında merak ettiğiniz her şeyi sorabilirsiniz!

Tarihi, mimarisi, gizemli Mithra Tapınağı veya nasıl gidileceği... Hepsini anlatabilirim! 😊</div>
    </div>
  </div>
  
  <div id="typing" class="typing">
    <span></span><span></span><span></span>
  </div>
  
  <div class="input-area">
    <input type="text" id="q" placeholder="Sorunuzu yazın..." onkeypress="if(event.key=='Enter')sendQ()">
    <button onclick="sendQ()">Gönder</button>
  </div>
  
  <div class="footer">
    🏛️ Roma Dönemi • MS 3. Yüzyıl • <a href="#">UNESCO Geçici Listesi</a>
  </div>
</div>

<script>
function ask(q) {
  document.getElementById('q').value = q;
  sendQ();
}

async function sendQ() {
  let q = document.getElementById('q').value.trim();
  if (!q) return;
  
  addMsg(q, 'user');
  document.getElementById('q').value = '';
  
  document.getElementById('typing').classList.add('show');
  
  try {
    let r = await fetch('/ask?q=' + encodeURIComponent(q));
    let d = await r.json();
    document.getElementById('typing').classList.remove('show');
    setTimeout(() => addMsg(d.answer, 'bot'), 200);
  } catch(e) {
    document.getElementById('typing').classList.remove('show');
    addMsg('Bağlantı hatası! Tekrar deneyin.', 'bot');
  }
}

function addMsg(txt, who) {
  let c = document.getElementById('chat');
  let m = document.createElement('div');
  m.className = 'msg ' + who;
  
  let av = document.createElement('div');
  av.className = 'avatar';
  av.textContent = who === 'bot' ? '🏛️' : '👤';
  
  let b = document.createElement('div');
  b.className = 'bubble';
  b.textContent = txt;
  
  m.appendChild(av);
  m.appendChild(b);
  c.appendChild(m);
  c.scrollTop = c.scrollHeight;
}
</script>
</body>
</html>
)rawliteral";

// ========== CAPTIVE PORTAL SAYFALARI ==========
const char captiveHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="0;url=http://192.168.4.1/">
<title>Zerzavan Kalesi</title>
</head>
<body>
<script>window.location.href='http://192.168.4.1/';</script>
</body>
</html>
)rawliteral";

// ========== HTTP HANDLERS ==========
void handleRoot() {
  server.send_P(200, "text/html", html);
}

void handleAsk() {
  if (server.hasArg("q")) {
    String cevap = getCevap(server.arg("q"));
    // JSON escape
    cevap.replace("\"", "\\\"");
    cevap.replace("\n", "\\n");
    String json = "{\"answer\":\"" + cevap + "\"}";
    server.send(200, "application/json", json);
  } else {
    server.send(400, "application/json", "{\"error\":\"Soru yok\"}");
  }
}

// Captive Portal - Tüm cihazlar için
void handleCaptive() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/html", "");
}

void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/html", "");
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════════════════╗");
  Serial.println("║   ZERZAVAN KALESİ - CAPTIVE PORTAL AI CHATBOT v4.0   ║");
  Serial.println("╠══════════════════════════════════════════════════════╣");
  Serial.println("║  ✓ Bağlam Hafızası (Evet/Hayır tanıma)               ║");
  Serial.println("║  ✓ Captive Portal (Otomatik sayfa açma)              ║");
  Serial.println("║  ✓ 11 Farklı Konu Kategorisi                         ║");
  Serial.println("║  ✓ 60+ Cevap Varyasyonu                              ║");
  Serial.println("╚══════════════════════════════════════════════════════╝\n");
  
  randomSeed(analogRead(0) + millis());
  
  // WiFi Access Point
  Serial.println("[*] WiFi Access Point başlatılıyor...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  delay(100);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("[+] IP Adresi: ");
  Serial.println(IP);
  
  // DNS Server - Captive Portal için
  Serial.println("[*] DNS Server başlatılıyor...");
  dnsServer.start(DNS_PORT, "*", IP);
  Serial.println("[+] DNS Server başlatıldı!");
  
  // HTTP Routes
  server.on("/", handleRoot);
  server.on("/ask", handleAsk);
  
  // Captive Portal Detection URLs
  server.on("/generate_204", handleCaptive);        // Android
  server.on("/gen_204", handleCaptive);             // Android
  server.on("/hotspot-detect.html", handleCaptive); // iOS
  server.on("/library/test/success.html", handleCaptive); // iOS
  server.on("/ncsi.txt", handleCaptive);            // Windows
  server.on("/connecttest.txt", handleCaptive);     // Windows
  server.on("/redirect", handleCaptive);            // Windows
  server.on("/success.txt", handleCaptive);         // Firefox
  server.on("/canonical.html", handleCaptive);      // Firefox
  server.on("/kindle-wifi/wifistub.html", handleCaptive); // Kindle
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("[+] Web Server başlatıldı!\n");
  
  Serial.println(">>> YENİ ÖZELLİKLER:");
  Serial.println("────────────────────────────────────────");
  Serial.println("🧠 BAĞLAM HAFIZASI:");
  Serial.println("   • 'Mithra hakkında bilgi ister misin?' → 'Evet' → Bilgi verir!");
  Serial.println("   • Son konuyu hatırlar");
  Serial.println("   • Takip sorularını yönetir");
  Serial.println("");
  Serial.println("📱 CAPTIVE PORTAL:");
  Serial.println("   • WiFi'ye bağlanınca sayfa otomatik açılır");
  Serial.println("   • Android, iOS, Windows, Firefox desteği");
  Serial.println("   • DNS yönlendirmesi aktif");
  Serial.println("────────────────────────────────────────\n");
  
  Serial.println(">>> KULLANIM:");
  Serial.println("1. Telefonunuzda WiFi'ye bağlanın: 'Zerzavan_Kalesi'");
  Serial.println("2. Sayfa otomatik açılacak!");
  Serial.println("3. Açılmazsa: http://192.168.4.1\n");
  
  Serial.println(">>> ÖRNEK DİYALOG:");
  Serial.println("Bot: 'Tarihi anlatayım mı?'");
  Serial.println("Siz: 'Evet' veya 'Olur' veya 'Anlat'");
  Serial.println("Bot: Tarihi anlatır! ✓\n");
  
  Serial.println("════════════════════════════════════════════════════════\n");
}

// ========== LOOP ==========
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
