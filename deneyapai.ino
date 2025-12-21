#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Zerzavan_Kalesi";
WebServer server(80);

// DURUM DEĞİŞKENLERİ
int dilModu = 0; // 0=Otomatik, 1=Türkçe, 2=İngilizce
String sonKonu = "";
int konusmaSayisi = 0;

// =====================================================
// ANAHTAR KELİME VE KONU TANIMLARI
// =====================================================

struct AnahtarKelime {
  String kelime;
  int agirlik;
};

struct Konu {
  String id;
  AnahtarKelime anahtarlar[15];
  int anahtarSayisi;
  String cevaplar[8];
  int cevapSayisi;
  String takipSorulari[4];
  int takipSayisi;
};

// TÜRKÇE KONULAR VE BİLGİ BANKASI
Konu tr_konular[] = {
  // 0 - SELAMLAMA
  {
    "selamlama",
    {{"merhaba", 100}, {"selam", 100}, {"hey", 90}, {"iyi günler", 95}, {"nasılsın", 80}, {"naber", 85}},
    6,
    {
      "Merhaba! 🏛️ Ben Zerzavan Kalesi'nin dijital rehberiyim. 1700 yıllık Roma tarihini keşfetmeye hazır mısınız? Size tarihi, mimarisi, gizemli Mithra Tapınağı veya günümüzdeki durumu hakkında bilgi verebilirim.",
      "Hoş geldiniz! 👋 Antik Roma'nın Anadolu'daki en önemli sınır kalelerinden birinde sizinle buluşmak harika. Ne öğrenmek istersiniz?",
      "Selam! 🌟 Zerzavan'ın büyüleyici dünyasına hoş geldiniz. Tarih, mimari, din veya arkeoloji... Hangi konuda yardımcı olabilirim?",
      "Merhaba değerli ziyaretçi! Roma İmparatorluğu'nun doğu sınırındaki bu muhteşem kale hakkında her şeyi öğrenmeye hazır olun!"
    },
    4,
    {"Kalenin tarihi hakkında bilgi ister misiniz?", "Mithra Tapınağı'nı merak ediyor musunuz?", "Mimarisi ilginizi çeker mi?"},
    3
  },
  
  // 1 - TARİH
  {
    "tarih",
    {{"tarih", 80}, {"tarihi", 85}, {"ne zaman", 90}, {"yapıldı", 85}, {"yapılmış", 85}, {"inşa", 80}, {"kurulmuş", 75}, {"yüzyıl", 70}, {"eski", 60}, {"antik", 70}, {"geçmiş", 65}, {"dönem", 60}},
    12,
    {
      "Zerzavan Kalesi, Milattan Sonra 3. yüzyılda Roma İmparatorluğu tarafından inşa edildi. Yaklaşık 1700 yıllık bu yapı, İmparator Diocletianus döneminde Sasani İmparatorluğu'na karşı doğu sınırını korumak için stratejik bir nokta olarak kuruldu. Roma'nın 'Limes Orientalis' adı verilen doğu savunma hattının en önemli kalelerinden biriydi.",
      "Kalenin tarihi MS 250-300 yılları arasına dayanıyor. Roma İmparatorluğu'nun en güçlü dönemlerinden birinde, Pers tehdidine karşı inşa edildi. Bizans döneminde de kullanılmaya devam etti ve yaklaşık 400 yıl boyunca aktif bir askeri üs olarak görev yaptı.",
      "3. yüzyıl Roma mühendisliğinin şaheseri! Kale, Mezopotamya'ya açılan kapının bekçisi olarak tasarlandı. Diocletianus'un Tetrarchy (Dörtlü Yönetim) sisteminde doğu sınırlarının korunmasında kritik rol oynadı.",
      "Antik çağın stratejik dehası bu kalede somutlaşıyor. İpek Yolu üzerindeki konumu sayesinde hem ticaret hem askeri açıdan büyük öneme sahipti. Roma tarihçileri kalenin hiç düşman eline geçmediğini yazar!"
    },
    4,
    {"Roma-Sasani ilişkileri hakkında daha fazla bilgi ister misiniz?", "Kalenin askeri önemi ilginizi çeker mi?"},
    2
  },
  
  // 2 - MİTHRA TAPINAĞI
  {
    "mithra",
    {{"mithra", 100}, {"mitra", 100}, {"tapınak", 85}, {"tapinak", 85}, {"din", 70}, {"tanrı", 75}, {"tanri", 75}, {"ibadet", 70}, {"tören", 70}, {"gizem", 65}, {"yeraltı", 60}, {"kült", 70}},
    12,
    {
      "Mithra Tapınağı, kalenin en gizemli ve büyüleyici bölümü! 🌟 Güneş tanrısı Mithra'ya adanan bu yeraltı tapınağı, Anadolu'daki en iyi korunmuş Mithraeum'lardan biri. Roma askerleri burada gizli inisiyasyon törenleri düzenlerdi. Sadece özel ritüellerden geçen askerler bu kutsal mekana girebilirdi.",
      "Mithraizm, Roma ordusunda son derece popüler bir gizem diniydi. Zerzavan'daki tapınak 12 metre uzunluğunda yeraltı odasına sahip. 7 farklı inisiyasyon seviyesi vardı: Kuzgun, Gelin, Asker, Aslan, Pers, Güneş Elçisi ve Pater (Baba). Her seviye farklı sırlar ve sorumluluklar içerirdi.",
      "Tapınakta kozmik düzen sembolize edilirdi. Duvarlarında boğa kurban sahneleri, yıldızlar ve zodyak işaretleri bulunurdu. Karanlıktan aydınlığa geçiş ritüelleri, askerlere cesaret ve sadakat aşılardı. Orijinal sunak ve ritüel platformları hala ayakta!",
      "Mithra kültü Hint-İran kökenli olup Roma lejyonerleri tarafından benimsenmiş. Gizlilik esastı - kadınlar katılamazdı. Askerler burada kardeşlik bağları kurardı. Tapınak, zorlu sınır görevindeki askerlerin manevi sığınağıydı."
    },
    4,
    {"İnisiyasyon törenleri hakkında daha fazla bilgi ister misiniz?", "Roma ordusundaki dinsel yaşam ilginizi çeker mi?"},
    2
  },
  
  // 3 - MİMARİ
  {
    "mimari",
    {{"mimari", 90}, {"yapı", 80}, {"bina", 75}, {"duvar", 80}, {"sur", 85}, {"kule", 80}, {"büyüklük", 70}, {"nasıl yapılmış", 85}, {"taş", 65}, {"inşaat", 70}, {"plan", 60}, {"teknik", 65}},
    12,
    {
      "Zerzavan Kalesi, 12 hektarlık devasa bir alanı kaplıyor! 🏗️ Çift sıra halinde 12-15 metre yüksekliğinde surları, 12 gözetleme kulesi, gelişmiş su sarnıçları ve yeraltı tünelleri mevcut. Bazalt taşlarından örülmüş duvarlar 3 metreye varan kalınlıkta - Roma mühendisliğinin zirvesi!",
      "Kale üç ana bölümden oluşuyor: askeri kışlalar, kutsal Mithra Tapınağı ve idari binalar. Ortogonal (dik açılı) planlama sistemi kullanılmış. Ana kapı, köprü ve hendek sistemi düşmanı yavaşlatacak şekilde tasarlanmış. İç kale ve dış kale olmak üzere iki savunma hattı var.",
      "Su mühendisliği hayranlık verici! Yağmur suyu toplama sistemleri, büyük sarnıçlar ve dağıtım kanalları sayesinde 300 asker ve aileleri aylarca kuşatmaya dayanabilirdi. Depreme dayanıklı inşaat teknikleri kullanılmış - 1700 yıl sonra hala ayakta!",
      "Stratejik tepe konumu sayesinde Dicle Vadisi kilometrelerce uzaktan görülebiliyordu. Bazalt kayaları özel olarak seçilmiş, taşlar birbirine kenetlenecek şekilde işlenmiş. Roma'nın taş işçiliği ve mühendislik bilgisinin canlı kanıtı!"
    },
    4,
    {"Su sistemleri hakkında daha fazla bilgi ister misiniz?", "Savunma yapıları ilginizi çeker mi?"},
    2
  },
  
  // 4 - ASKERİ
  {
    "askeri",
    {{"asker", 90}, {"askeri", 90}, {"ordu", 85}, {"lejyon", 90}, {"savunma", 80}, {"savaş", 75}, {"garnizon", 85}, {"komutan", 70}, {"silah", 65}, {"strateji", 70}, {"düşman", 65}, {"nöbet", 60}},
    12,
    {
      "Zerzavan, yaklaşık 300 Roma lejyonerinin sürekli konuşlandığı stratejik bir garnizondu. ⚔️ Sasani İmparatorluğu'na karşı erken uyarı sistemi ve ilk savunma hattı görevi görüyordu. Gözetleme kuleleri sayesinde düşman hareketleri anında tespit edilir, ateş ve duman sinyalleriyle haber iletilirdi.",
      "Kale, Roma'nın Doğu Sınır Komutanlığı (Dux Orientis) kontrolündeydi. Askeri hiyerarşi çok netti: Praefectus (komutan), Centurion'lar (yüzbaşılar), Legionarius'lar (lejyonerler) ve Auxilia (yardımcı birlikler). Disiplin ve düzen her şeyden önemliydi.",
      "Roma-Sasani savaşlarında kritik rol oynadı! Hem savunma hem de saldırı üssü olarak kullanıldı. Bölgedeki diğer Roma birliklerine lojistik destek sağlardı. İpek Yolu güvenliğini de kontrol ederek ticaret kervanlarını korurdu.",
      "Lejyonerlerin günlük yaşamı çok düzenliydi: Şafakta kalkış, sabah eğitimleri, silah bakımı, nöbet, inşaat çalışmaları... Akşamları Mithra Tapınağı'nda toplanır, dini ritüeller ve kardeşlik törenleri yaparlardı. Sınır görevi zorlu ama onurluydu!"
    },
    4,
    {"Roma lejyonerleri hakkında daha fazla bilgi ister misiniz?", "Sasani tehdidi hakkında bilgi vermemi ister misiniz?"},
    2
  },
  
  // 5 - GÜNÜMÜZ
  {
    "gunumuz",
    {{"günümüz", 90}, {"gunumuz", 90}, {"bugün", 85}, {"bugun", 85}, {"şimdi", 80}, {"simdi", 80}, {"ziyaret", 85}, {"giriş", 75}, {"açık", 70}, {"müze", 70}, {"turizm", 75}, {"unesco", 85}},
    12,
    {
      "Günümüzde Zerzavan, UNESCO Dünya Mirası Geçici Listesi'nde yer alıyor! 🌍 Her yıl binlerce yerli ve yabancı turist bu eşsiz tarihi dokuyu keşfetmeye geliyor. Kültür ve Turizm Bakanlığı tarafından koruma altında. Giriş ücretsiz ve yıl boyunca ziyarete açık!",
      "Aktif arkeolojik kazılar Mardin Müzesi başkanlığında devam ediyor. Doç. Dr. Aytaç Coşkun liderliğindeki ekip her yıl yeni keşifler yapıyor. 2014'ten bu yana binlerce eser gün yüzüne çıkarıldı. Restorasyon çalışmaları bilimsel yöntemlerle sürdürülüyor.",
      "Turizm açısından muhteşem bir destinasyon! Dicle Vadisi'ne hakim eşsiz manzarası var. Güneş batımında fotoğraf çekmek için ideal. Yakında modern bir ziyaretçi merkezi açılması planlanıyor. Sanal tur imkanları da geliştiriliyor.",
      "2020'de UNESCO Dünya Mirası Geçici Listesi'ne alındı - kalıcı listeye alınması için çalışmalar sürüyor. Türkiye'nin en önemli arkeolojik alanlarından biri. Bölge ekonomisine turizm geliri sağlıyor, yerel halk da faydalanıyor."
    },
    4,
    {"Kazı çalışmaları hakkında bilgi ister misiniz?", "Nasıl ulaşabileceğinizi anlatayım mı?"},
    2
  },
  
  // 6 - KONUM/ULAŞIM
  {
    "konum",
    {{"nerede", 95}, {"konum", 90}, {"yer", 70}, {"adres", 85}, {"nasıl gidilir", 90}, {"ulaşım", 90}, {"ulasim", 90}, {"yol", 65}, {"mesafe", 70}, {"diyarbakır", 80}, {"çınar", 85}, {"cinar", 85}},
    12,
    {
      "Zerzavan Kalesi, Diyarbakır ili Çınar ilçesinde, şehir merkezine sadece 13 km mesafede! 📍 Demirölçek köyü yakınlarında, stratejik bir tepe üzerinde konumlanmış. Dicle Vadisi'ne hakim muhteşem manzarası var.",
      "Ulaşım çok kolay! Diyarbakır'dan arabayla yaklaşık 20 dakika sürüyor. D-950 karayolunu takip edip Çınar istikametine gidin, tabelalara uyun. Çınar ilçe merkezine sadece 3 km mesafede. GPS'e 'Zerzavan Kalesi' yazarak kolayca bulabilirsiniz.",
      "Coğrafi koordinatlar: 37°43' Kuzey, 40°26' Doğu. Deniz seviyesinden yaklaşık 660 metre yükseklikte. Dicle Nehri'nin 5 km doğusunda. Bölgenin en yüksek tepelerinden birinde - bu yüzden savunma için ideal seçilmiş!",
      "Antik çağda İpek Yolu'nun önemli bir kavşağındaydı. Roma yol ağının (Via Militaris) kritik noktası. Bugün Diyarbakır'ın kültür turizmi rotasının vazgeçilmez durağı. Toplu taşıma sınırlı, kendi aracınızla gitmeniz önerilir."
    },
    4,
    {"Yakın çevrede başka gezilecek yerler ister misiniz?", "Ziyaret saatleri hakkında bilgi vermemi ister misiniz?"},
    2
  },
  
  // 7 - ROMA İMPARATORLUĞU
  {
    "roma",
    {{"roma", 90}, {"romalı", 85}, {"imparatorluk", 80}, {"latin", 70}, {"diocletianus", 85}, {"bizans", 75}, {"batı", 60}, {"doğu", 60}},
    8,
    {
      "Roma İmparatorluğu döneminde bu bölge stratejik açıdan çok kritikti! 🦅 Mezopotamya'ya açılan kapı görevi görüyordu. Doğuda güçlü Sasani İmparatorluğu ile sürekli rekabet vardı. Zerzavan bu rekabetin ön cephesindeydi.",
      "İmparator Diocletianus (MS 284-305) döneminde doğu sınırları yeniden organize edildi. Zerzavan bu yeni savunma planının kilit taşlarından biriydi. Tetrarchy (Dörtlü Yönetim) sistemi döneminde önem kazandı.",
      "Roma sadece askerlik değil, kültür de yayıyordu. Latin dili, Roma hukuku, mimari tarzlar, dinsel pratikler... Hepsi Zerzavan gibi sınır kalelerinden Anadolu'ya yayıldı. Kültürel bir erime potasıydı!",
      "Lejyonerler Anadolu, Balkanlar, Suriye, hatta Britanya'dan gelirdi. Kozmopolit bir topluluk oluşturmuşlardı. Roma vatandaşlığı, birleştirici kimlikti. 25 yıl hizmetten sonra emeklilik ve toprak hakkı kazanırlardı."
    },
    4,
    {"Sasani İmparatorluğu hakkında bilgi ister misiniz?", "Roma lejyonerleri hakkında daha fazla bilgi vermemi ister misiniz?"},
    2
  },
  
  // 8 - SASANİ
  {
    "sasani",
    {{"sasani", 95}, {"pers", 85}, {"iran", 80}, {"düşman", 70}, {"doğu", 60}, {"savaş", 65}},
    6,
    {
      "Sasani İmparatorluğu (MS 224-651), Roma'nın doğudaki en büyük ve en güçlü rakibiydi! 🏹 Pers kültürünün varisi olan Sasaniler, güçlü zırhlı süvarileri (Cataphract) ve okçularıyla Roma'ya sürekli meydan okuyordu.",
      "Roma-Sasani savaşları yüzyıllarca sürdü. Zerzavan gibi sınır kaleleri bu çatışmaların merkezindeydi. Bazen barış anlaşmaları yapılsa da gerginlik hiç bitmedi. Her iki imparatorluk da Mezopotamya kontrolü için savaştı.",
      "Sasaniler İran platosunu, Mezopotamya'yı ve İpek Yolu'nun büyük kısmını kontrol ediyordu. Roma ile hem askeri hem ticari rekabet içindeydiler. Zerdüştlük resmi dinleriydi.",
      "İlginç olan şu: Rakip olmalarına rağmen kültürler birbirini etkiledi. Mimari teknikler, askeri taktikler, sanat stilleri karşılıklı olarak aktarıldı. Zerzavan bu kültürel etkileşimin canlı tanığı!"
    },
    4,
    {"Roma-Sasani savaşları hakkında daha fazla bilgi ister misiniz?"},
    1
  },
  
  // 9 - ARKEOLOJİK KAZILAR
  {
    "kazi",
    {{"kazı", 95}, {"kazi", 95}, {"arkeoloji", 90}, {"arkeolog", 85}, {"keşif", 80}, {"kesif", 80}, {"bulgu", 75}, {"eser", 70}, {"buluntu", 75}},
    9,
    {
      "Arkeolojik kazılar 2014 yılında başladı ve büyük heyecanla devam ediyor! 🔍 Mardin Müzesi başkanlığında, Doç. Dr. Aytaç Coşkun yönetiminde bilimsel kazılar yapılıyor. Her sezon yeni sürprizler ortaya çıkıyor.",
      "Şimdiye kadar Mithra Tapınağı, kışla yapıları, su sarnıçları, idari binalar gün yüzüne çıkarıldı. Binlerce seramik parçası, metal eşya, Roma sikkeleri, mühürler bulundu. Her bulgu tarihe yeni bir pencere açıyor!",
      "Modern arkeoloji teknikleri kullanılıyor: Jeofizik tarama, yer radarı (GPR), manyetik ölçümler... Böylece kazmadan önce yeraltında ne olduğu tespit edilebiliyor. Lazer tarama ile 3D modeller oluşturuluyor.",
      "Bulunan eserler Mardin Müzesi'nde sergileniyor. Bazı özel parçalar restorasyon laboratuvarlarında işleniyor. Uluslararası bilimsel dergilerde makaleler yayınlanıyor, kongrelerde sunumlar yapılıyor. Akademik dünyada büyük ilgi görüyor!"
    },
    4,
    {"Bulunan eserler hakkında daha fazla bilgi ister misiniz?"},
    1
  },
  
  // 10 - SU SİSTEMLERİ
  {
    "su",
    {{"su", 85}, {"sarnıç", 95}, {"sarnic", 95}, {"kuyu", 80}, {"kanal", 75}, {"içme", 70}},
    6,
    {
      "Zerzavan'ın su mühendisliği Roma dehasının kanıtı! 💧 Büyük sarnıçlar, yağmur suyu toplama sistemleri ve dağıtım kanalları mevcut. 300 asker ve ailelerinin aylarca yetecek su depolanabiliyordu.",
      "Kuşatma durumlarında su hayati önemdeydi. Roma mühendisleri bunu çok iyi biliyordu. Sarnıçlar kayaya oyulmuş, sızdırmaz sıva ile kaplanmış. Bazıları hala su tutabiliyor!",
      "Yağmur suyu çatılardan ve avlulardan özel kanallarla toplanırdı. Filtre sistemleri vardı. Ayrıca derin kuyular da kazılmıştı. Su tasarrufu için katı kurallar uygulanırdı."
    },
    3,
    {"Kalenin mimarisi hakkında daha fazla bilgi ister misiniz?"},
    1
  },

  // 11 - GENEL BİLGİ / NE BU
  {
    "genel",
    {{"ne", 70}, {"nedir", 85}, {"hakkında", 80}, {"anlat", 85}, {"bilgi", 75}, {"öğren", 70}, {"merak", 65}, {"kale", 75}, {"zerzavan", 90}},
    9,
    {
      "Zerzavan Kalesi, Diyarbakır'da bulunan 1700 yıllık muhteşem bir Roma askeri üssüdür! 🏛️ MS 3. yüzyılda Sasani tehdidine karşı inşa edilmiş. 12 hektarlık alanda surlar, kuleler, kışlalar ve gizemli Mithra Tapınağı bulunuyor. UNESCO Dünya Mirası Geçici Listesi'nde!",
      "Kısaca: Roma İmparatorluğu'nun doğu sınırındaki en önemli kalelerden biri. Askeri garnizon, dini merkez ve stratejik gözetleme noktası olarak hizmet vermiş. Bugün Türkiye'nin en değerli arkeolojik alanlarından biri!",
      "Zerzavan, antik dünyanın askeri mühendislik harikası! Savunma yapıları, su sistemleri ve yeraltı Mithra Tapınağı ile Roma'nın gücünü ve bilgisini gözler önüne seriyor. Her yıl binlerce ziyaretçi ağırlıyor."
    },
    3,
    {"Tarihi hakkında daha fazla bilgi ister misiniz?", "Mithra Tapınağı'nı anlatayım mı?", "Nasıl gidileceğini söyleyeyim mi?"},
    3
  }
};

// İNGİLİZCE KONULAR
Konu en_konular[] = {
  // 0 - GREETING
  {
    "greeting",
    {{"hello", 100}, {"hi", 100}, {"hey", 90}, {"good", 80}, {"greetings", 85}},
    5,
    {
      "Hello! 🏛️ I'm the digital guide of Zerzavan Castle. Ready to explore 1700 years of Roman history? I can tell you about its history, architecture, mysterious Mithra Temple, or current state.",
      "Welcome! 👋 It's great to meet you at one of the most important Roman border fortresses in Anatolia. What would you like to learn?",
      "Hi there! 🌟 Welcome to the fascinating world of Zerzavan. History, architecture, religion, archaeology... How can I help you?"
    },
    3,
    {"Would you like to know about the castle's history?", "Are you curious about the Mithra Temple?"},
    2
  },
  
  // 1 - HISTORY
  {
    "history",
    {{"history", 90}, {"when", 85}, {"built", 85}, {"old", 70}, {"ancient", 80}, {"century", 75}, {"past", 65}},
    7,
    {
      "Zerzavan Castle was built in the 3rd century AD by the Roman Empire. This magnificent 1700-year-old structure was established as a strategic point to protect the eastern border against the Sassanian Empire. It was one of the most important fortresses of Rome's 'Limes Orientalis' defense line.",
      "The castle dates back to 250-300 AD. Built during one of Rome's most powerful periods against the Persian threat. It continued to be used during the Byzantine period and served as an active military base for about 400 years.",
      "A masterpiece of 3rd-century Roman engineering! The castle was designed as the guardian of the gateway to Mesopotamia. It played a critical role in protecting the eastern borders during Diocletian's Tetrarchy system."
    },
    3,
    {"Would you like more information about Roman-Sassanian relations?"},
    1
  },
  
  // 2 - MITHRA
  {
    "mithra",
    {{"mithra", 100}, {"temple", 85}, {"religion", 75}, {"god", 70}, {"worship", 70}, {"mystery", 75}, {"underground", 70}},
    7,
    {
      "The Mithra Temple is the most mysterious part of the castle! 🌟 Dedicated to the sun god Mithra, this underground temple is one of the best-preserved Mithreums in Anatolia. Roman soldiers held secret initiation ceremonies here.",
      "Mithraism was an extremely popular mystery religion in the Roman army. The temple at Zerzavan has a 12-meter underground chamber. There were 7 different initiation levels: Raven, Bride, Soldier, Lion, Persian, Sun Courier, and Pater (Father).",
      "The temple symbolized cosmic order. Bull sacrifice scenes, stars, and zodiac signs adorned its walls. Rituals of passing from darkness to light instilled courage and loyalty in soldiers."
    },
    3,
    {"Would you like more information about initiation ceremonies?"},
    1
  },
  
  // 3 - ARCHITECTURE  
  {
    "architecture",
    {{"architecture", 90}, {"structure", 80}, {"building", 75}, {"wall", 80}, {"tower", 80}, {"how built", 85}, {"stone", 70}},
    7,
    {
      "Zerzavan Castle covers a massive area of 12 hectares! 🏗️ Double rows of walls 12-15 meters high, 12 watchtowers, advanced water cisterns, and underground tunnels. Walls built from basalt stones reach up to 3 meters thick - the pinnacle of Roman engineering!",
      "The castle consists of three main sections: military barracks, sacred Mithra Temple, and administrative buildings. An orthogonal planning system was used. The defense system with main gate, bridge, and moat was designed to slow down enemies.",
      "The water engineering is admirable! Rainwater collection systems, large cisterns, and distribution channels could sustain 300 soldiers and their families for months under siege."
    },
    3,
    {"Would you like to know more about the water systems?"},
    1
  },
  
  // 4 - MILITARY
  {
    "military",
    {{"military", 90}, {"army", 85}, {"soldier", 85}, {"legion", 90}, {"defense", 80}, {"war", 75}, {"garrison", 85}},
    7,
    {
      "Zerzavan was a strategic garrison where about 300 Roman legionaries were permanently stationed. ⚔️ It served as an early warning system and first line of defense against the Sassanian Empire.",
      "The castle was under the control of Rome's Eastern Border Command (Dux Orientis). Military hierarchy was very clear: Praefectus (commander), Centurions, Legionaries, and Auxilia (auxiliary units).",
      "It played a critical role in Roman-Sassanian wars! Used as both a defensive and offensive base. It provided logistical support to other Roman units in the region."
    },
    3,
    {"Would you like to know more about Roman legionaries?"},
    1
  },
  
  // 5 - TODAY
  {
    "today",
    {{"today", 90}, {"now", 85}, {"current", 80}, {"visit", 85}, {"tourism", 80}, {"unesco", 90}, {"open", 70}},
    7,
    {
      "Today, Zerzavan is on the UNESCO World Heritage Tentative List! 🌍 Thousands of visitors come every year. Under protection by the Ministry of Culture and Tourism. Free entrance and open year-round!",
      "Active archaeological excavations continue under the leadership of Mardin Museum. New discoveries are made every season. Scientific restoration work is ongoing.",
      "A wonderful tourism destination! Unique view overlooking the Tigris Valley. Perfect for sunset photography. A modern visitor center is planned."
    },
    3,
    {"Would you like information about the excavations?"},
    1
  },
  
  // 6 - LOCATION
  {
    "location",
    {{"where", 95}, {"location", 90}, {"address", 85}, {"how to get", 90}, {"directions", 85}, {"distance", 70}},
    6,
    {
      "Zerzavan Castle is located in Çınar district of Diyarbakır province, only 13 km from the city center! 📍 Near Demirölçek village, on a strategic hilltop with magnificent views of the Tigris Valley.",
      "Getting there is easy! About 20 minutes by car from Diyarbakır. Follow the D-950 highway towards Çınar. Type 'Zerzavan Castle' in GPS to find it easily.",
      "Geographic coordinates: 37°43' North, 40°26' East. About 660 meters above sea level. 5 km east of the Tigris River."
    },
    3,
    {"Would you like to know about visiting hours?"},
    1
  },
  
  // 7 - GENERAL
  {
    "general",
    {{"what", 80}, {"about", 75}, {"tell", 80}, {"information", 75}, {"castle", 80}, {"zerzavan", 90}},
    6,
    {
      "Zerzavan Castle is a magnificent 1700-year-old Roman military base in Diyarbakır! 🏛️ Built in the 3rd century AD against the Sassanian threat. Walls, towers, barracks, and the mysterious Mithra Temple spread across 12 hectares. On the UNESCO World Heritage Tentative List!",
      "In short: One of the most important fortresses on the eastern border of the Roman Empire. Served as a military garrison, religious center, and strategic observation point. Today one of Turkey's most valuable archaeological sites!",
      "Zerzavan is an ancient military engineering marvel! Defense structures, water systems, and underground Mithra Temple showcase Rome's power and knowledge."
    },
    3,
    {"Would you like to know more about its history?", "Shall I tell you about the Mithra Temple?"},
    2
  }
};

int tr_konuSayisi = 12;
int en_konuSayisi = 8;

// =====================================================
// YARDIMCI FONKSİYONLAR
// =====================================================

bool isTurkish(String s) {
  if(dilModu == 1) return true;
  if(dilModu == 2) return false;
  
  s.toLowerCase();
  
  // Türkçe karakterler
  if(s.indexOf("ç")!=-1 || s.indexOf("ğ")!=-1 || s.indexOf("ı")!=-1 ||
     s.indexOf("ö")!=-1 || s.indexOf("ş")!=-1 || s.indexOf("ü")!=-1 ||
     s.indexOf("İ")!=-1 || s.indexOf("Ş")!=-1 || s.indexOf("Ğ")!=-1) return true;
  
  // Türkçe kelimeler
  String trKelimeler[] = {"nedir", "nerede", "nasıl", "nasil", "anlat", "hakkında", "hakkinda", 
                          "merhaba", "selam", "evet", "hayır", "hayir", "lütfen", "lutfen",
                          "teşekkür", "tesekkur", "kale", "tarih", "yapı", "yapi"};
  String enKelimeler[] = {"what", "where", "how", "tell", "about", "the", "hello", "hi", 
                          "yes", "no", "please", "thank", "castle", "history", "is", "are"};
  
  int trSayac = 0, enSayac = 0;
  for(int i=0; i<20; i++) {
    if(s.indexOf(trKelimeler[i]) != -1) trSayac++;
  }
  for(int i=0; i<16; i++) {
    if(s.indexOf(enKelimeler[i]) != -1) enSayac++;
  }
  
  return trSayac >= enSayac;
}

String toLowerCase(String s) {
  s.toLowerCase();
  return s;
}

bool kelimeIceriyor(String cumle, String kelime) {
  cumle = toLowerCase(cumle);
  kelime = toLowerCase(kelime);
  return cumle.indexOf(kelime) != -1;
}

// Kullanıcının cümlesinden anahtar kelimeyi çıkar
String kullaniciReferansi(String soru, bool tr) {
  soru = toLowerCase(soru);
  
  if(kelimeIceriyor(soru, "mithra") || kelimeIceriyor(soru, "mitra")) {
    return tr ? "Mithra Tapınağı" : "Mithra Temple";
  }
  if(kelimeIceriyor(soru, "tarih") || kelimeIceriyor(soru, "history")) {
    return tr ? "tarihi" : "history";
  }
  if(kelimeIceriyor(soru, "mimari") || kelimeIceriyor(soru, "architecture")) {
    return tr ? "mimarisi" : "architecture";
  }
  if(kelimeIceriyor(soru, "asker") || kelimeIceriyor(soru, "military") || kelimeIceriyor(soru, "ordu")) {
    return tr ? "askeri yapısı" : "military structure";
  }
  if(kelimeIceriyor(soru, "roma") || kelimeIceriyor(soru, "roman")) {
    return tr ? "Roma dönemi" : "Roman period";
  }
  if(kelimeIceriyor(soru, "sasani") || kelimeIceriyor(soru, "pers")) {
    return tr ? "Sasaniler" : "Sassanians";
  }
  if(kelimeIceriyor(soru, "kazı") || kelimeIceriyor(soru, "kazi") || kelimeIceriyor(soru, "excavation")) {
    return tr ? "kazı çalışmaları" : "excavations";
  }
  if(kelimeIceriyor(soru, "su") || kelimeIceriyor(soru, "water") || kelimeIceriyor(soru, "sarnıç")) {
    return tr ? "su sistemleri" : "water systems";
  }
  
  return "";
}

// Bağlamsal giriş cümleleri
String baglamGiris(String soru, int konuIndex, bool tr) {
  soru = toLowerCase(soru);
  
  // Selamlama için özel
  if(konuIndex == 0) return "";
  
  String referans = kullaniciReferansi(soru, tr);
  
  // Soru tiplerine göre girişler
  if(kelimeIceriyor(soru, "nedir") || kelimeIceriyor(soru, "what is")) {
    if(referans != "") {
      return tr ? "Ah, " + referans + " hakkında merak ediyorsunuz! Harika bir soru. " 
                : "Ah, you're curious about " + referans + "! Great question. ";
    }
    return tr ? "Çok güzel bir soru! " : "That's a great question! ";
  }
  
  if(kelimeIceriyor(soru, "anlat") || kelimeIceriyor(soru, "tell")) {
    if(referans != "") {
      return tr ? referans + " hakkında anlatayım: " 
                : "Let me tell you about " + referans + ": ";
    }
    return tr ? "Size anlatabileceğim çok şey var! " : "I have so much to tell you! ";
  }
  
  if(kelimeIceriyor(soru, "nasıl") || kelimeIceriyor(soru, "nasil") || kelimeIceriyor(soru, "how")) {
    return tr ? "İlginç bir soru! " : "Interesting question! ";
  }
  
  if(kelimeIceriyor(soru, "nerede") || kelimeIceriyor(soru, "where")) {
    return tr ? "Konum bilgisi için: " : "For location information: ";
  }
  
  if(kelimeIceriyor(soru, "ne zaman") || kelimeIceriyor(soru, "when")) {
    return tr ? "Zamanlama açısından: " : "In terms of timing: ";
  }
  
  if(kelimeIceriyor(soru, "neden") || kelimeIceriyor(soru, "why")) {
    return tr ? "Bunun ilginç bir nedeni var! " : "There's an interesting reason for that! ";
  }
  
  // Konu bazlı girişler
  String girisler_tr[] = {
    "", // selamlama
    "Tarihi gerçekten büyüleyici! ", // tarih
    "Mithra Tapınağı benim de favorim! ", // mithra
    "Mimarisi muhteşem! ", // mimari
    "Askeri önemi çok büyük! ", // askeri
    "Günümüzde çok popüler! ", // gunumuz
    "Konumu stratejik! ", // konum
    "Roma dönemi çok önemli! ", // roma
    "Sasaniler güçlü rakipti! ", // sasani
    "Kazılar çok heyecanlı! ", // kazi
    "Su sistemi harika! ", // su
    "Çok güzel bir soru! " // genel
  };
  
  String girisler_en[] = {
    "",
    "The history is fascinating! ",
    "The Mithra Temple is my favorite too! ",
    "The architecture is magnificent! ",
    "Its military importance is huge! ",
    "Very popular today! ",
    "Strategic location! ",
    "The Roman period is crucial! ",
    "Sassanians were powerful rivals! ",
    "The excavations are exciting! ",
    "The water system is amazing! ",
    "Great question! "
  };
  
  if(konuIndex < 12) {
    return tr ? girisler_tr[konuIndex] : girisler_en[min(konuIndex, 7)];
  }
  
  return "";
}

// Ana konu eşleştirme fonksiyonu
int konuBul(String soru, bool tr) {
  soru = toLowerCase(soru);
  
  Konu* konular = tr ? tr_konular : en_konular;
  int konuSayisi = tr ? tr_konuSayisi : en_konuSayisi;
  
  int enYuksekPuan = 0;
  int enIyiKonu = -1;
  
  for(int k = 0; k < konuSayisi; k++) {
    int toplamPuan = 0;
    
    for(int a = 0; a < konular[k].anahtarSayisi; a++) {
      if(kelimeIceriyor(soru, konular[k].anahtarlar[a].kelime)) {
        toplamPuan += konular[k].anahtarlar[a].agirlik;
      }
    }
    
    if(toplamPuan > enYuksekPuan) {
      enYuksekPuan = toplamPuan;
      enIyiKonu = k;
    }
  }
  
  // Minimum eşik
  if(enYuksekPuan < 60) {
    // Genel bilgi konusuna yönlendir
    return tr ? 11 : 7;
  }
  
  return enIyiKonu;
}

// =====================================================
// ANA CEVAP ÜRETİCİ
// =====================================================

String getCevap(String soru) {
  bool tr = isTurkish(soru);
  
  // Boş soru kontrolü
  if(soru.length() < 2) {
    return tr ? "Lütfen bir soru sorun, size yardımcı olmaya hazırım! 😊" 
              : "Please ask a question, I'm ready to help! 😊";
  }
  
  // Konu bul
  int konuIndex = konuBul(soru, tr);
  
  if(konuIndex == -1) {
    return tr ? "Üzgünüm, bu konuda yeterli bilgim yok. Zerzavan Kalesi'nin tarihi, mimarisi, Mithra Tapınağı, askeri önemi veya konumu hakkında sorabilirsiniz!" 
              : "Sorry, I don't have enough information about that. You can ask about Zerzavan Castle's history, architecture, Mithra Temple, military importance, or location!";
  }
  
  Konu* konular = tr ? tr_konular : en_konular;
  
  // Bağlamsal giriş
  String giris = baglamGiris(soru, konuIndex, tr);
  
  // Rastgele cevap seç
  int cevapIndex = random(0, konular[konuIndex].cevapSayisi);
  String anaCevap = konular[konuIndex].cevaplar[cevapIndex];
  
  // Takip sorusu ekle (bazen)
  String takip = "";
  if(konular[konuIndex].takipSayisi > 0 && random(0, 3) == 0) {
    int takipIndex = random(0, konular[konuIndex].takipSayisi);
    takip = "\n\n" + konular[konuIndex].takipSorulari[takipIndex];
  }
  
  // Konuşma sayısını artır
  konusmaSayisi++;
  sonKonu = konular[konuIndex].id;
  
  return giris + anaCevap + takip;
}

// =====================================================
// HTML ARAYÜZ
// =====================================================

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Zerzavan Kalesi AI Rehber</title>
<style>
* { margin: 0; padding: 0; box-sizing: border-box; }

:root {
  --primary: #8B4513;
  --primary-dark: #654321;
  --secondary: #D4A574;
  --accent: #C9A66B;
  --bg-dark: #2C1810;
  --bg-light: #F5E6D3;
  --text-dark: #1a1a1a;
  --text-light: #f5f5f5;
}

body {
  font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
  background: linear-gradient(135deg, var(--bg-dark) 0%, #4A2C17 50%, var(--primary-dark) 100%);
  min-height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 10px;
}

.container {
  background: linear-gradient(180deg, #FDF8F3 0%, #F5E6D3 100%);
  border-radius: 24px;
  box-shadow: 0 25px 80px rgba(0,0,0,0.5), inset 0 1px 0 rgba(255,255,255,0.5);
  max-width: 800px;
  width: 100%;
  overflow: hidden;
  animation: slideUp 0.6s ease-out;
  border: 3px solid var(--accent);
}

@keyframes slideUp {
  from { opacity: 0; transform: translateY(30px); }
  to { opacity: 1; transform: translateY(0); }
}

.header {
  background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
  color: var(--text-light);
  padding: 30px 25px;
  text-align: center;
  position: relative;
  overflow: hidden;
}

.header::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: url("data:image/svg+xml,%3Csvg width='60' height='60' viewBox='0 0 60 60' xmlns='http://www.w3.org/2000/svg'%3E%3Cg fill='none' fill-rule='evenodd'%3E%3Cg fill='%23ffffff' fill-opacity='0.05'%3E%3Cpath d='M36 34v-4h-2v4h-4v2h4v4h2v-4h4v-2h-4zm0-30V0h-2v4h-4v2h4v4h2V6h4V4h-4zM6 34v-4H4v4H0v2h4v4h2v-4h4v-2H6zM6 4V0H4v4H0v2h4v4h2V6h4V4H6z'/%3E%3C/g%3E%3C/g%3E%3C/svg%3E");
  opacity: 0.3;
}

.header h1 {
  font-size: 2em;
  margin-bottom: 8px;
  font-weight: 700;
  text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
  position: relative;
}

.header h1::before {
  content: '🏛️';
  margin-right: 10px;
}

.header p {
  font-size: 1em;
  opacity: 0.95;
  position: relative;
}

.badge-container {
  display: flex;
  gap: 10px;
  justify-content: center;
  margin-top: 15px;
  flex-wrap: wrap;
  position: relative;
}

.badge {
  background: rgba(255,255,255,0.2);
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 0.8em;
  backdrop-filter: blur(5px);
  border: 1px solid rgba(255,255,255,0.3);
}

.lang-btns {
  display: flex;
  gap: 8px;
  justify-content: center;
  margin-top: 18px;
  position: relative;
}

.lang-btn {
  padding: 10px 18px;
  background: rgba(255,255,255,0.15);
  border: 2px solid rgba(255,255,255,0.5);
  color: white;
  border-radius: 25px;
  cursor: pointer;
  font-size: 14px;
  font-weight: 600;
  transition: all 0.3s;
  backdrop-filter: blur(5px);
}

.lang-btn:hover {
  background: rgba(255,255,255,0.25);
  transform: translateY(-2px);
}

.lang-btn.active {
  background: white;
  color: var(--primary-dark);
  border-color: white;
}

.chat {
  padding: 25px;
  max-height: 400px;
  min-height: 350px;
  overflow-y: auto;
  background: linear-gradient(180deg, #FFFFFF 0%, #FDF8F3 100%);
}

.chat::-webkit-scrollbar { width: 6px; }
.chat::-webkit-scrollbar-track { background: #f1f1f1; border-radius: 3px; }
.chat::-webkit-scrollbar-thumb { background: var(--secondary); border-radius: 3px; }

.msg {
  margin-bottom: 18px;
  animation: msgSlide 0.4s ease-out;
  display: flex;
  align-items: flex-start;
  gap: 12px;
}

@keyframes msgSlide {
  from { opacity: 0; transform: translateY(15px); }
  to { opacity: 1; transform: translateY(0); }
}

.msg.user { justify-content: flex-end; }

.avatar {
  width: 42px;
  height: 42px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 20px;
  flex-shrink: 0;
  box-shadow: 0 3px 10px rgba(0,0,0,0.15);
}

.msg.bot .avatar {
  background: linear-gradient(135deg, var(--primary), var(--primary-dark));
}

.msg.user .avatar {
  background: linear-gradient(135deg, var(--secondary), var(--accent));
  order: 2;
}

.bubble {
  padding: 14px 18px;
  border-radius: 18px;
  max-width: 75%;
  line-height: 1.6;
  font-size: 15px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.08);
}

.msg.user .bubble {
  background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
  color: white;
  border-bottom-right-radius: 4px;
}

.msg.bot .bubble {
  background: white;
  color: var(--text-dark);
  border-bottom-left-radius: 4px;
  border: 1px solid #e8dfd6;
}

.typing {
  display: none;
  padding: 14px 18px;
  background: white;
  border-radius: 18px;
  border-bottom-left-radius: 4px;
  width: fit-content;
  border: 1px solid #e8dfd6;
  margin-left: 54px;
}

.typing.show { display: block; }

.typing span {
  display: inline-block;
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: var(--primary);
  margin: 0 2px;
  animation: typingBounce 1.4s infinite ease-in-out;
}

.typing span:nth-child(2) { animation-delay: 0.2s; }
.typing span:nth-child(3) { animation-delay: 0.4s; }

@keyframes typingBounce {
  0%, 60%, 100% { transform: translateY(0); }
  30% { transform: translateY(-8px); }
}

.quick-btns {
  padding: 12px 25px;
  background: #FDF8F3;
  border-top: 1px solid #e8dfd6;
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  justify-content: center;
}

.quick-btn {
  background: white;
  color: var(--primary-dark);
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 13px;
  cursor: pointer;
  border: 2px solid var(--secondary);
  transition: all 0.2s;
  font-weight: 500;
}

.quick-btn:hover {
  background: var(--primary);
  color: white;
  border-color: var(--primary);
  transform: translateY(-2px);
}

.input-area {
  padding: 20px 25px;
  background: white;
  display: flex;
  gap: 12px;
  border-top: 1px solid #e8dfd6;
}

input {
  flex: 1;
  padding: 14px 20px;
  border: 2px solid #e8dfd6;
  border-radius: 25px;
  font-size: 15px;
  outline: none;
  transition: all 0.3s;
  background: #FDF8F3;
}

input:focus {
  border-color: var(--primary);
  background: white;
  box-shadow: 0 0 0 3px rgba(139,69,19,0.1);
}

input::placeholder {
  color: #a09080;
}

button.send-btn {
  padding: 14px 28px;
  background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
  color: white;
  border: none;
  border-radius: 25px;
  font-size: 15px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s;
  display: flex;
  align-items: center;
  gap: 8px;
}

button.send-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(139,69,19,0.35);
}

button.send-btn:active {
  transform: translateY(0);
}

.footer {
  background: var(--bg-dark);
  color: rgba(255,255,255,0.7);
  padding: 15px 25px;
  text-align: center;
  font-size: 12px;
}

.footer a {
  color: var(--accent);
  text-decoration: none;
}

@media (max-width: 600px) {
  .header h1 { font-size: 1.5em; }
  .bubble { max-width: 85%; font-size: 14px; }
  .quick-btn { font-size: 12px; padding: 6px 12px; }
  button.send-btn { padding: 12px 20px; }
  .badge { font-size: 0.7em; padding: 4px 10px; }
}
</style>
</head>
<body>

<div class="container">
  <div class="header">
    <h1>Zerzavan Kalesi</h1>
    <p>Yapay Zeka Destekli Dijital Rehber</p>
    <div class="badge-container">
      <span class="badge">🏺 MS 3. Yüzyıl</span>
      <span class="badge">🌍 UNESCO Geçici Listesi</span>
      <span class="badge">📍 Diyarbakır</span>
    </div>
    <div class="lang-btns">
      <button class="lang-btn active" onclick="setLang(0)">🤖 Otomatik</button>
      <button class="lang-btn" onclick="setLang(1)">🇹🇷 Türkçe</button>
      <button class="lang-btn" onclick="setLang(2)">🇬🇧 English</button>
    </div>
  </div>
  
  <div class="chat" id="chat">
    <div class="msg bot">
      <div class="avatar">🏛️</div>
      <div class="bubble">Hoş geldiniz! 👋 Ben Zerzavan Kalesi'nin dijital rehberiyim. 1700 yıllık Roma tarihini birlikte keşfedelim! Size kalenin tarihi, mimarisi, gizemli Mithra Tapınağı veya nasıl ziyaret edebileceğiniz hakkında bilgi verebilirim. Ne öğrenmek istersiniz?</div>
    </div>
  </div>
  
  <div class="quick-btns">
    <span class="quick-btn" onclick="askQ('Kalenin tarihi nedir?')">📜 Tarihi</span>
    <span class="quick-btn" onclick="askQ('Mithra Tapınağı hakkında bilgi ver')">🌟 Mithra</span>
    <span class="quick-btn" onclick="askQ('Mimarisi nasıl?')">🏗️ Mimari</span>
    <span class="quick-btn" onclick="askQ('Nerede, nasıl gidilir?')">📍 Konum</span>
    <span class="quick-btn" onclick="askQ('Askeri önemi neydi?')">⚔️ Askeri</span>
    <span class="quick-btn" onclick="askQ('Günümüzde durumu nasıl?')">🎫 Günümüz</span>
  </div>
  
  <div class="input-area">
    <input type="text" id="q" placeholder="Sorunuzu yazın... (örn: Kale ne zaman yapıldı?)" onkeypress="if(event.key=='Enter')ask()">
    <button class="send-btn" onclick="ask()">
      <span>Gönder</span>
      <span>📤</span>
    </button>
  </div>
  
  <div class="footer">
    🏛️ Zerzavan Kalesi Dijital Rehber Sistemi | <a href="#">Diyarbakır Kültür ve Turizm</a>
  </div>
</div>

<script>
let langMode = 0;

function setLang(mode) {
  langMode = mode;
  fetch('/setlang?m=' + mode);
  document.querySelectorAll('.lang-btn').forEach((b, i) => {
    b.classList.toggle('active', i === mode);
  });
}

function askQ(question) {
  document.getElementById('q').value = question;
  ask();
}

async function ask() {
  let q = document.getElementById('q').value.trim();
  if (!q) return;
  
  addMsg(q, 'user');
  document.getElementById('q').value = '';
  showTyping();
  
  try {
    let r = await fetch('/ask?q=' + encodeURIComponent(q));
    let d = await r.json();
    hideTyping();
    setTimeout(() => addMsg(d.answer, 'bot'), 200);
  } catch(e) {
    hideTyping();
    addMsg('Bağlantı hatası! Lütfen tekrar deneyin.', 'bot');
  }
}

function addMsg(txt, who) {
  let c = document.getElementById('chat');
  let m = document.createElement('div');
  m.className = 'msg ' + who;
  
  let avatar = document.createElement('div');
  avatar.className = 'avatar';
  avatar.textContent = who === 'bot' ? '🏛️' : '👤';
  
  let b = document.createElement('div');
  b.className = 'bubble';
  b.innerHTML = txt.replace(/\n/g, '<br>');
  
  m.appendChild(avatar);
  m.appendChild(b);
  c.appendChild(m);
  c.scrollTop = c.scrollHeight;
}

function showTyping() {
  let c = document.getElementById('chat');
  let t = document.createElement('div');
  t.id = 'typing';
  t.className = 'typing show';
  t.innerHTML = '<span></span><span></span><span></span>';
  c.appendChild(t);
  c.scrollTop = c.scrollHeight;
}

function hideTyping() {
  let t = document.getElementById('typing');
  if(t) t.remove();
}

// Sayfa yüklendiğinde input'a focus
document.addEventListener('DOMContentLoaded', () => {
  document.getElementById('q').focus();
});
</script>
</body>
</html>
)rawliteral";

// =====================================================
// WEB SERVER İŞLEYİCİLERİ
// =====================================================

void handleRoot() {
  server.send_P(200, "text/html", html);
}

void handleAsk() {
  if (server.hasArg("q")) {
    String soru = server.arg("q");
    String cevap = getCevap(soru);
    
    // JSON için özel karakterleri escape et
    cevap.replace("\\", "\\\\");
    cevap.replace("\"", "\\\"");
    cevap.replace("\n", "\\n");
    cevap.replace("\r", "");
    
    String json = "{\"answer\":\"" + cevap + "\"}";
    server.send(200, "application/json", json);
  } else {
    server.send(400, "application/json", "{\"error\":\"Soru bulunamadi\"}");
  }
}

void handleSetLang() {
  if (server.hasArg("m")) {
    dilModu = server.arg("m").toInt();
    server.send(200, "text/plain", "OK");
  }
}

// =====================================================
// SETUP VE LOOP
// =====================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n");
  Serial.println("╔══════════════════════════════════════════════════════════╗");
  Serial.println("║     ZERZAVAN KALESİ YAPAY ZEKA REHBERİ v4.0              ║");
  Serial.println("║     Geliştirilmiş Bağlamsal Konuşma Sistemi              ║");
  Serial.println("╚══════════════════════════════════════════════════════════╝");
  Serial.println();
  
  randomSeed(analogRead(0) + millis());
  
  Serial.println("[*] WiFi Access Point başlatılıyor...");
  WiFi.softAP(ssid);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("[+] Bağlantı IP: ");
  Serial.println(IP);
  
  server.on("/", handleRoot);
  server.on("/ask", handleAsk);
  server.on("/setlang", handleSetLang);
  
  server.begin();
  Serial.println("[+] Web server aktif!\n");
  
  Serial.println("┌─────────────────────────────────────────────────────────┐");
  Serial.println("│ YENİ ÖZELLİKLER:                                        │");
  Serial.println("├─────────────────────────────────────────────────────────┤");
  Serial.println("│ ✓ Gelişmiş anahtar kelime ağırlıklandırma sistemi       │");
  Serial.println("│ ✓ Bağlamsal giriş cümleleri                             │");
  Serial.println("│ ✓ Kullanıcı referansı algılama                          │");
  Serial.println("│ ✓ Takip soruları sistemi                                │");
  Serial.println("│ ✓ 12 farklı konu kategorisi (TR)                        │");
  Serial.println("│ ✓ 80+ farklı cevap varyasyonu                           │");
  Serial.println("│ ✓ Modern antik tema arayüzü                             │");
  Serial.println("│ ✓ Hızlı erişim butonları                                │");
  Serial.println("│ ✓ Responsive mobil tasarım                              │");
  Serial.println("└─────────────────────────────────────────────────────────┘");
  Serial.println();
  Serial.println("KULLANIM:");
  Serial.println("  1. WiFi'a bağlanın: 'Zerzavan_Kalesi'");
  Serial.println("  2. Tarayıcı: http://192.168.4.1");
  Serial.println("  3. Sorularınızı doğal dilde sorun!");
  Serial.println();
  Serial.println("══════════════════════════════════════════════════════════════\n");
}

void loop() {
  server.handleClient();
  delay(1);
}
