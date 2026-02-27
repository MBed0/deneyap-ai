#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* ssid = "Zerzevan Kalesi Yapay Zeka";
const byte DNS_PORT = 53;

WebServer server(80);
DNSServer dnsServer;

// ========== BAĞLAM HAFIZASI ==========
struct BaglamHafiza {
  String sonKonu;
  String bekleyenKonu;
  String sonSoru;
  String sonCevap;
  int konusmaSayisi;
  bool takipSorusuVar;
  unsigned long sonZaman;
};

BaglamHafiza hafiza = {"", "", "", "", 0, false, 0};

// ========== KONU YAPISI ==========
struct Konu {
  String id;
  String anahtar[25];
  int agirlik[25];
  int anahtarSayisi;
  String cevaplar[8];
  int cevapSayisi;
  String takipSorulari[4];
  int takipSayisi;
};

// ========== ONAY / RED KELİMELERİ ==========
String onayKelimeleri[] = {"evet", "yes", "olur", "tamam", "ok", "okay", "istiyorum",
                           "isterim", "tabii", "tabi", "sure", "peki", "anlat",
                           "söyle", "soyle", "devam", "lutfen", "lütfen"};
int onayKelimeSayisi = 18;

String redKelimeleri[] = {"hayır", "hayir", "no", "istemiyorum", "istemem", "yok", "gerek yok"};
int redKelimeSayisi = 7;

// ========== DATASET: DOCX'TAN ALINAN TÜRKÇE KONULAR ==========
Konu konular[] = {

  // 0 - SELAMLAMA
  {"selamlama",
    {"merhaba", "selam", "hey", "hello", "hi", "günaydın", "iyi günler", "naber", "nasılsın", "hosgeldin"},
    {100, 100, 90, 100, 90, 95, 95, 80, 70, 85},
    10,
    {
      "Merhaba! 🏰 Ben Zerzevan Kalesi'nin dijital rehberiyim. Doç. Dr. Aytaç Coşkun'un akademik araştırmalarına dayanan bu sistemle kalenin 1700 yıllık tarihini keşfedebilirsiniz!",
      "Selam! Hoş geldiniz! Dicle Üniversitesi Arkeoloji Bölümü'nün 2014'ten bu yana yürüttüğü kazı çalışmalarına dayanan bilgilerle Zerzevan'ı tanıyalım.",
      "Merhaba değerli ziyaretçi! 🏛️ Zerzevan Kalesi; Asur, Pers, Parth, Roma ve Bizans dönemlerinin izlerini taşıyan eşsiz bir miras alanıdır.",
      "Hoş geldiniz! Roma İmparatorluğu'nun en doğu sınır garnizonu Zerzevan hakkında merak ettiğiniz her şeyi sorabilirsiniz!"
    }, 4,
    {"Kalenin tarihini mi, mimarisini mi, yoksa dünyaca ünlü Mithras Tapınağı'nı mı merak ediyorsunuz?",
     "Hangi konu ilginizi çekiyor? Tarih, mimari, kazılar, konum?",
     "Size önce kalenin tarihçesini mi anlatsam?"}, 3
  },

  // 1 - TARİH & KRONOLOJİ
  {"tarih",
    {"tarih", "tarihi", "history", "ne zaman", "yapıldı", "yapildi", "inşa", "insa", "kaç yıl", "eski",
     "antik", "geçmiş", "kuruluş", "kurulus", "dönem", "yüzyıl", "milat", "asur", "severus"},
    {80, 85, 80, 70, 75, 75, 70, 70, 65, 50, 60, 55, 70, 70, 75, 70, 65, 85, 80},
    19,
    {
      "Zerzevan Kalesi'nin tarihi Asur Dönemi'ne (MÖ 882-611) kadar uzanır! 🏛️ Pers Dönemi'nde (MÖ 550-331) Kral Yolu üzerinde güvenlik noktası olarak kullanılmış, Parth Dönemi (MÖ 140-85) buluntuları da alanın bu dönemde aktif olduğunu kanıtlamaktadır.",
      "Kalenin asıl askeri yerleşimi MS 3. yüzyılda Severuslar Dönemi'nde (MS 198-235) inşa edildi. Surlar ve yapılar Anastasios I (MS 491-518) ve Justinianos I (MS 527-565) dönemlerinde onarılarak son halini aldı. 639 yılında İslam orduları bölgeyi fethede kadar kesintisiz kullanıldı.",
      "İlk bilimsel ziyaret 1766'da Carsten Niebuhr tarafından yapıldı. 1880'de Eduard Sachau, 1910'da Conrad Preusser, 1911'de Samuel Guyer kısa incelemeler gerçekleştirdi. Sistematik arkeolojik kazılar ise 2014 yılında Doç. Dr. Aytaç Coşkun liderliğinde başladı ve hâlâ devam ediyor.",
      "1890'larda bir ailenin kaleye yerleşmesiyle yeniden iskân başladı ve 17 haneye ulaştı. 1967'de su sıkıntısı ve ulaşım zorluğu nedeniyle halk, 1 km mesafedeki Demirölçek Köyü'nü kurdu. 2020'de kale UNESCO Dünya Mirası Geçici Listesi'ne alındı."
    }, 4,
    {"Kalenin mimarisini de öğrenmek ister misiniz?",
     "Mithras Tapınağı'nın keşif sürecini anlatayım mı?",
     "Askeri yapısını merak ediyor musunuz?"}, 3
  },

  // 2 - MITHRAS TAPINAGI
  {"mithra",
    {"mithra", "mithras", "mitra", "tapınak", "tapinak", "temple", "din", "tanrı", "tanri", "ibadet",
     "gizem", "tören", "toren", "yeraltı", "yeralti", "gizli", "mithraeum", "kutsal", "ritüel", "ritual"},
    {100, 100, 100, 85, 85, 80, 60, 55, 55, 65, 70, 65, 65, 70, 70, 60, 100, 75, 80, 80},
    20,
    {
      "Mithras Tapınağı (Mithraeum) Zerzevan'ın en büyük keşfi! 🔮 2017 yılında ortaya çıkarılan tapınak, dünya üzerindeki en iyi korunmuş Mithras tapınaklarından biridir. Mithras, Hint-Pers kökenli bir tanrıdır; Roma'nın doğu seferlerinde bu inançla tanışılmış ve askerlere yayılmıştır.",
      "Tapınak 2016'da önce yer altı yapısı olarak tespit edildi. 14x7 m boyutlarında, 4,3 m yüksekliğindeki bu yapının tavanında 5 kare açıklık ve döner merdivenli giriş bulunmaktadır. 2017'de daha kutsal bölümü ortaya çıkarıldı: Çözümlenemeyen yazıtlar ve sembollerle bezeli bir koridor ve tapınak odası!",
      "Tapınak kompleksi birden fazla yapıdan oluşmaktadır. Boğa kurban törenlerinin ve ritüellerin tapınak dışındaki yapıda gerçekleştirildiği anlaşılmıştır. Roma'nın doğu sınırındaki bu Mithraeum, imparatorluk sınırları içindeki en eski tapınaklardan birisi olma özelliği taşımaktadır.",
      "Zerzevan Kalesi; hem pagan Mithras tapınağı hem de Hristiyan kilisesi barındıran dünyadaki nadir garnizonlardan biridir! Mithras inancındaki Zervan/Zurvan (Pers zaman tanrısı) ile Mithras'ın yakın ilişkisi, Zerzevan isminin de bu inançtan türemiş olabileceğine işaret etmektedir."
    }, 4,
    {"Tapınakla bağlantılı yer altı yapısını anlatayım mı?",
     "Kalenin kilisesi hakkında da bilgi vermemi ister misiniz?",
     "Kazı sürecini merak ediyor musunuz?"}, 3
  },

  // 3 - MİMARİ & YAPILAR
  {"mimari",
    {"mimari", "architecture", "yapı", "yapi", "bina", "duvar", "kule", "sur", "taş", "tas",
     "büyüklük", "buyukluk", "nasıl yapılmış", "inşaat", "insaat", "mühendislik", "burç", "burc", "kaya", "blok"},
    {90, 85, 70, 70, 60, 75, 75, 70, 60, 60, 55, 55, 80, 65, 65, 70, 80, 80, 70, 65},
    20,
    {
      "Zerzevan Kalesi 60 dönüm sur içi alana sahip! 🏗️ Ova seviyesinden 124 m yükseklikte kayalık bir tepe üzerine kurulmuş. Sur dışındaki nekropol, su kanalları, sunu çanakları ve taş ocakları ile birlikte yaklaşık 10.000 dönüm alan izlenebilmektedir.",
      "Tüm yerleşim 12-15 m yüksekliğinde, 2,1-3,2 m kalınlığındaki surlarla çevrilmiştir. 1,2 km uzunluğundaki sur duvarında 10 burç ve 2 kule tespit edilmiştir. Güney kulesi 19,2 m korunmuş yüksekliğiyle orijinalinin 21 m olduğu hesaplanmıştır.",
      "Güney kule (gözetleme/savunma), kilise, yönetim binası, arsenal, kaya sunağı güney bölgede; cadde-sokaklar ve konutlar kuzey bölgede yer almaktadır. Konutların bodrum katı ahır/depo, üst katı ise asker ve siviller için yaşam alanı olarak kullanılmıştır.",
      "Yerleşim, castrum (Roma askeri kalesi) geleneğinden farklı olarak topografyaya göre şekillendirilmiş özgün bir mimariye sahiptir. Antik Amida ve Dara arasındaki bu Roma castrumunun benzeri bulunmamaktadır. Ana kayadan yontularak elde edilen bloklar sur duvarlarında kullanılmıştır."
    }, 4,
    {"Su sistemini anlatmamı ister misiniz?",
     "Arsenal yapısı hakkında bilgi vermemi ister misiniz?",
     "Konutların yapısını merak ediyor musunuz?"}, 3
  },

  // 4 - ASKERİ YAPI
  {"askeri",
    {"asker", "askeri", "military", "ordu", "army", "lejyon", "savunma", "savaş", "savas", "garnizon",
     "komutan", "silah", "strateji", "nöbet", "nobet", "arsenal", "sınır", "sinir", "limes"},
    {85, 90, 85, 75, 75, 80, 80, 70, 70, 85, 65, 60, 70, 55, 55, 80, 80, 80, 75},
    19,
    {
      "Zerzevan, Roma İmparatorluğu'nun doğu sınır garnizonu olarak Amida (Diyarbakır)'dan Dara (Mardin)'ya giden stratejik yol üzerindeydi. ⚔️ Aynı zamanda Edessa (Şanlıurfa)-Nisibis (Nusaybin) güzergahında da aktif görev üstlenmekteydi.",
      "Sasani hükümdarı II. Şapur, MS 359'da bu antik yolu kullanarak Amida'yı ele geçirdi. Bu tarihten sonra doğuda garnizon kentler kurulmaya başlandı. Zerzevan bu savunma hattının kritik noktalarından biriydi.",
      "Arsenal yapısı dar-uzun formda, iki bölümlü, 10,9x36,4 m boyutlarındadır. Beşik çatılı bu silah deposunun kuzey duvarında giriş kapısı izi görülmektedir. 1975'e kadar ayakta olan doğu duvarı o tarihte yıkılmıştır.",
      "Askeri kompleks; garnizonu, tapınakları, kiliseleri, konutları ve idari binaları ile sivil ve askeri nüfusu bir arada barındırıyordu. Zerzevan sadece askerler için değil, onlara hizmet eden siviller için de yaşam alanıydı."
    }, 4,
    {"Roma-Sasani çatışmaları hakkında daha fazla bilgi ister misiniz?",
     "Kalenin surlarını ve kulelerini anlatayım mı?",
     "Arsenal yapısı hakkında detaylı bilgi vermemi ister misiniz?"}, 3
  },

  // 5 - KONUM & ULAŞIM
  {"konum",
    {"nerede", "where", "konum", "location", "yer", "adres", "nasıl gidilir", "nasil gidilir",
     "ulaşım", "ulasim", "yol", "diyarbakır", "diyarbakir", "çınar", "cinar", "demirölçek", "koordinat"},
    {95, 90, 90, 85, 60, 70, 85, 85, 75, 75, 55, 70, 70, 75, 75, 80, 70},
    17,
    {
      "Zerzevan Kalesi, Diyarbakır ili Çınar ilçesinin 13 km güneydoğusunda, Demirölçek Köyü sınırları içindedir. 📍 Ova seviyesinden 124 m yüksekteki kayalık tepede, Dicle Nehri'ne hakim stratejik konumuyla kurulmuştur.",
      "Antik dönemde Amida (Diyarbakır)-Dara (Mardin) yolu ile Edessa (Şanlıurfa)-Nisibis (Nusaybin) güzergahının kesişiminde yer alıyordu. Bu stratejik pozisyon hem ticaret hem de askeri güvenlik açısından son derece kritikti.",
      "Diyarbakır şehir merkezinden araçla yaklaşık 20 dakika mesafede. D-950 karayolundan Çınar istikametine gidip tabelaları takip edebilirsiniz. GPS'e 'Zerzevan Kalesi' yazmanız yeterli.",
      "Zerzevan ismi Kürtçe'deki Zêr/altın ve Zîv/gümüş kelimelerinden türetilmiş olabilir; köylüler kaleyi 'Altın Şehir' olarak da tanımlamaktadır. Antik ismi ise Sammachi ya da Sardebar olabileceği öne sürülmektedir."
    }, 4,
    {"Günümüzdeki durumunu ve ziyaret bilgilerini anlatayım mı?",
     "Kalenin tarihsel önemini merak ediyor musunuz?",
     "Yakındaki antik kentler (Amida, Dara) hakkında bilgi ister misiniz?"}, 3
  },

  // 6 - GÜNÜMÜZ & KAZILER
  {"gunumuz",
    {"günümüz", "gunumuz", "bugün", "bugun", "şimdi", "simdi", "ziyaret", "visit", "turizm",
     "müze", "muze", "unesco", "restorasyon", "kazı", "kazi", "2014", "arkeoloji", "keşif"},
    {85, 85, 70, 70, 65, 65, 80, 75, 75, 70, 70, 90, 75, 85, 85, 80, 80, 75},
    18,
    {
      "Zerzevan Kalesi ve Mithraeum 2020 yılında UNESCO Dünya Mirası Geçici Listesi'ne alındı! 🌍 Kalıcı listeye alınması için çalışmalar sürmektedir. Diyarbakır Valiliği koordinasyonunda Dicle Üniversitesi Arkeoloji Bölümü kazıları yürütmektedir.",
      "2014 yılında başlayan kazılar; 2016'da yer altı yapısının, 2017'de Mithras Tapınağı'nın keşfiyle büyük ivme kazandı. Her sezon yeni bulgular ortaya çıkmakta, tapınak kompleksindeki çalışmalar hâlâ devam etmektedir.",
      "Arkeolojik kazılarda karşılaşılan önemli eserler arasında İstanbul Arkeoloji Müzesi'nde 852 envanter numarasıyla kayıtlı bronz bir kova bulunmaktadır. Üzerindeki Yunanca yazıt MS 6. yüzyıla tarihlendirilmektedir.",
      "Kale giriş ücretsizdir ve ziyarete açıktır. Dicle Vadisi'ne hakim konumuyla eşsiz bir manzara sunar. Alandaki restorasyon çalışmaları bilimsel yöntemlerle titizlikle sürdürülmektedir."
    }, 4,
    {"Kazılarda ortaya çıkan Mithras Tapınağı hakkında daha fazla bilgi ister misiniz?",
     "UNESCO süreciyle ilgili daha fazla bilgi vermemi ister misiniz?",
     "Kalenin tarihini baştan anlatayım mı?"}, 3
  },

  // 7 - SU SİSTEMİ & SARNIÇLAR
  {"su",
    {"su", "water", "sarnıç", "sarnic", "kuyu", "well", "cistern", "kaynak", "kanal", "depo", "tonoz"},
    {75, 70, 90, 90, 80, 75, 85, 65, 80, 70, 75},
    11,
    {
      "Zerzevan'da 63 adet ana kayaya oyulmuş sarnıç tespit edilmiştir! 💧 Konutların önünde bireysel sarnıçlar, kuzey bölümde ise kamuya ait büyük sarnıçlar bulunmaktadır. Bu tonozlu sarnıçların alt bölümü kemer başlangıcına kadar ana kayaya oyulmuştur.",
      "Ana su deposu 11,2x22,5 m boyutlarında, iki bölümlü, üzeri tonozlarla örtülü büyük sarnıçlardır. Doğu duvarı sura dayanmaktadır. Güney surunun dışındaki 616 m uzunluğunda ortaya çıkarılan kanal bu büyük sarnıçlara su sağlamaktaydı.",
      "Kanalın genişliği 51-68 cm, derinliği 48-61 cm arasındadır. Sarnıçlardan suyu çekebilmek için tavana çıkrık benzeri bir düzenek için açıklıklar yapılmıştır. Bu gelişmiş su yönetimi uzun kuşatmalara dayanmayı mümkün kılıyordu."
    }, 3,
    {"Kalenin mimarisini de anlatayım mı?",
     "Askeri yapı hakkında bilgi ister misiniz?"}, 2
  },

  // 8 - KİLİSE & HIRİSTİYANLIK
  {"kilise",
    {"kilise", "church", "hristiyan", "hıristiyan", "vaftiz", "haç", "dua", "ibadet", "din", "yazıt",
     "yeraltı kilisesi", "bizans"},
    {100, 95, 90, 90, 85, 80, 70, 65, 55, 75, 100, 80},
    12,
    {
      "Zerzevan'da iki kilise tespit edilmiştir. İlki, yerleşim merkezinde Roma Dönemi kaya mezarından dönüştürülmüş bir yeraltı kilisesidir. MS 2-3. yüzyılda kullanılan bu mezar, Hristiyanlığın serbest bırakılmasından sonra küçük bir cemaat tarafından ibadete açılmıştır.",
      "Yeraltı kilisesinde doğu kline haç işlenerek dönüştürülmüş, duvarlara haç ve yazıtlar kazınmıştır. Dışında ana kayaya oyulmuş bir vaftiz havuzu ve kandil nişi bulunmaktadır. Bu, Zerzevan'ın ilk Hristiyan cemaatinin ibadet yeridir.",
      "Büyük kamu kilisesi ise yerleşimin en yüksek güney noktasına MS 6. yüzyılda inşa edilmiştir. Doğu-batı doğrultusunda 4 mekândan oluşan kilisede bir vaftiz havuzu bulunmaktadır. Bu yapı cemaat sayısının artmasıyla inşa edilmiş olmalıdır."
    }, 3,
    {"Mithras Tapınağı ile kiliseler arasındaki ilişkiyi anlatayım mı?",
     "Kalenin tüm yapılarını özetleyeyim mi?"}, 2
  },

  // 9 - NEKROPOL & MEZARLAR
  {"nekropol",
    {"mezar", "nekropol", "ölü", "gömü", "kaya mezarı", "tonozlu", "lahit", "defin", "cemetery"},
    {90, 100, 80, 75, 95, 90, 85, 75, 80},
    9,
    {
      "Sur dışında üç tür mezar barındıran bir nekropol alanı bulunmaktadır: tonozlu mezarlar, klineli kaya mezarları ve lahit biçiminde oyulmuş kaya mezarları. Mezarlara basamaklı girişlerle ulaşılmakta, oda içindeki üç yönde ölülerin yatırılması için klineler açılmıştır.",
      "Araştırmalarda iki adet tonozlu mezar tespit edilmiştir. Kaya mezarlarının 4,5 m kuzeybatısında ise kuzeybatı-güneydoğu doğrultusunda 5 adet lahit biçiminde oyulmuş mezar bulunmaktadır. Mezarların lahit kapakları günümüze ulaşamamıştır.",
      "Sunu çanakları da dikkat çekicidir: Güney kulenin 202 m güneydoğusundaki tepede 85-58 cm çapında, 24-28 cm derinliğinde iki çanak bulunmaktadır. Bu alanda Roma döneminde dini törenler yapılmış olmalıdır."
    }, 3,
    {"Kalenin mimari yapısını anlatayım mı?",
     "Roma dini uygulamaları hakkında bilgi ister misiniz?"}, 2
  },

  // 10 - ROMA İMPARATORLUĞU
  {"roma",
    {"roma", "roman", "rome", "imparatorluk", "empire", "latin", "diocletianus", "bizans",
     "byzantine", "anastasios", "justinianos", "severus", "constantius"},
    {85, 80, 75, 70, 65, 55, 75, 70, 65, 80, 85, 80, 80},
    13,
    {
      "Zerzevan, Roma İmparatorluğu'nun en doğu sınırındaki garnizonu olarak Sasani tehdidine karşı kritik bir işlev üstlendi. Özellikle Anastasios I (MS 491-518) ve Justinianos I (MS 527-565) dönemlerinde surlar ve yapılar yenilenmiştir.",
      "Antik yazar Procopius, Justinianos döneminde Dara ve Amida arasındaki kalelerin yeniden inşa edildiğini anlatır; ancak Zerzevan'dan bahsetmez. Bu durum kalenin Justinianos'tan önce inşa edildiğine işaret etmektedir.",
      "Roma'nın castrum geleneğinden farklı olarak Zerzevan topografyaya göre şekillendirilmiştir. Geometrik planlama yerine arazi koşullarına uygun özgün bir mimari sergilemektedir. Bu, onu diğer Roma garnizonu kalelerinden ayıran en temel özelliktir.",
      "Kale MS 639'da İslam orduları tarafından bölgenin fethine kadar kullanıldı. Bu tarihten sonra jeopolitik önemini yitirdi ve 1890'lara kadar geçici barınak olmaktan öteye gidemedi."
    }, 4,
    {"Sasani İmparatorluğu hakkında bilgi ister misiniz?",
     "Kalenin Bizans Dönemi'ni anlatayım mı?",
     "Justinianos dönemi yapı çalışmalarını merak ediyor musunuz?"}, 3
  },

  // 11 - SASANİ İMPARATORLUĞU
  {"sasani",
    {"sasani", "sassanian", "pers", "persian", "iran", "düşman", "dusman", "rakip",
     "savas", "şapur", "shapur", "parth", "parthian"},
    {100, 95, 80, 75, 70, 65, 65, 60, 55, 90, 85, 80, 75},
    13,
    {
      "Sasani İmparatorluğu, Roma'nın doğudaki en büyük rakibiydi! ⚔️ Pers kültürünün mirasçısı olan Sasaniler, güçlü süvari birlikleri ve gelişmiş kuşatma teknolojisiyle Roma'ya sürekli meydan okuyordu. Zerzevan bu çatışmanın tam ön cephesindeydi.",
      "Sasani hükümdarı II. Şapur MS 359'da Amida-Dara antik yolunu kullanarak Amida'yı ele geçirdi. Bu olay doğudaki garnizon ağının genişletilmesini hızlandırdı ve Zerzevan'ın önemini daha da artırdı.",
      "Parth Dönemi (MÖ 140-85) buluntuları, Zerzevan'ın Sasaniler öncesinde de önemli bir güzergah üzerinde olduğunu kanıtlar. Roma-Sasani sınırı Zerzevan'ın hemen yakınından geçmekteydi.",
      "İlginç bir nokta: Roma ve Sasani kültürleri birbirini derinden etkiledi. Mithras inancı bu etkileşimin en somut örneğidir; Hint-Pers kökenli bu din Roma askerleri arasında yayıldı ve Zerzevan'daki tapınak bu sentezin simgesidir."
    }, 4,
    {"Roma İmparatorluğu hakkında daha fazla bilgi ister misiniz?",
     "Kalenin askeri yapısını anlatayım mı?",
     "MS 359 olaylarını detaylı aktarayım mı?"}, 3
  },

  // 12 - YARDIM
  {"yardim",
    {"yardım", "yardim", "help", "ne sorabilirim", "neler var", "özellik", "komut", "nasıl kullanılır", "ara", "search"},
    {100, 100, 95, 90, 85, 70, 75, 85, 80, 80},
    10,
    {
      "Size yardımcı olmaktan mutluluk duyarım! 📚\n\n📜 Tarih: 'Tarihi nedir?', 'Ne zaman yapıldı?'\n🔮 Mithras: 'Tapınak nedir?', 'Mithraeum'\n🏛️ Mimari: 'Surlar kaç metre?', 'Arsenal nedir?'\n⚔️ Askeri: 'Garnizon', 'Savunma'\n📍 Konum: 'Nerede?', 'Nasıl gidilir?'\n🌍 Günümüz: 'Kazılar', 'UNESCO'\n💧 Su: 'Sarnıçlar', 'Kanal'\n⛪ Kilise: 'Hristiyan', 'Yeraltı kilisesi'\n🔍 Arama: Belirli bir kelimeyi metin içinde aramak için kullanın!",
      "Ben Zerzevan Kalesi hakkında her konuda bilgi verebilirim! Tarih, mimari, Mithras tapınağı, askeri yapı, kiliseler, nekropol, su sistemi, konum ve günümüzdeki durum... İstediğinizi sorun!"
    }, 2,
    {"Nereden başlamak istersiniz?"}, 1
  }
};

int konuSayisi = 13;

// =============================================
// DATASET: DOCX İÇERİĞİ (Kelime Arama için)
// =============================================

// Metin parçaları - docx'tan alınan önemli bilgiler
const char* dataset[] = {
  // Genel
  "Zerzevan Kalesi Diyarbakır ili Çınar ilçesinin 13 km güneydoğusunda Demirölçek Köyü sınırları içindedir.",
  "Zerzevan Kalesi ova seviyesinden 124 m yükseklikteki kayalık tepe üzerine kurulmuştur.",
  "Sur içi alan 60 dönümdür. Surların dışındaki alanlarla birlikte yaklaşık 10 bin dönüm izlenebilir.",
  "Zerzevan ismi Kürtçe Zêr (altın) ve Zîv (gümüş) kelimelerinden türetilmiş olabilir.",
  "Köylüler kaleyi Altın Şehir olarak tanımlamaktadır.",
  "Kalenin antik ismi Sammachi ya da Sardebar olabileceği öne sürülmektedir.",
  // Tarih / Kronoloji
  "Zerzevan Kalesi tarihi arkeolojik bulgulara göre Asur Dönemi MÖ 882-611 yıllarına kadar uzanmaktadır.",
  "Pers Dönemi MÖ 550-331 yıllarında Kral Yolu üzerindeki yerleşim yol güvenliği için kullanılmıştır.",
  "Parth Dönemi MÖ 140-85 buluntuları alanın bu dönemde kullanıldığını göstermektedir.",
  "Asıl askeri yerleşim MS 3. yüzyılda Severuslar Dönemi MS 198-235 yıllarında inşa edilmiştir.",
  "Surlar ve yapılar Anastasios I MS 491-518 ve Justinianos I MS 527-565 dönemlerinde onarılmıştır.",
  "639 yılında İslam orduları tarafından bölgenin fethine kadar yerleşim kesintisiz kullanılmıştır.",
  "1890 yıllarında bir aile kaleye yerleşti ve yerleşim 17 haneye ulaştı.",
  "1967 yılında köy halkı su ve ulaşım sorunu nedeniyle Demirölçek Köyünü kurdu.",
  "Zerzevan Kalesi ve Mithraeum 2020 yılında UNESCO Dünya Mirası Geçici Listesi ne alındı.",
  // Araştırma tarihi
  "Yerleşim ilk defa 1766 yılında Carsten Niebuhr tarafından ziyaret edilmiştir.",
  "Niebuhr yerleşimi Kasr Zerzaua olarak adlandırmıştır.",
  "1880 yılında Eduard Sachau yerleşimi ziyaret etmiştir.",
  "1910 yılında Conrad Preusser kısa bir ziyaret gerçekleştirmiştir.",
  "1911 yılında Samuel Guyer gözlemlerini inceleme yazısı olarak kaleme almıştır.",
  "Sistematik arkeolojik kazılar 2014 yılında Doç. Dr. Aytaç Coşkun öncülüğünde başlamıştır.",
  "Kazılar Dicle Üniversitesi Arkeoloji Bölümü tarafından yürütülmektedir.",
  // Mimari
  "Bütün yerleşim 12-15 m yüksekliğinde 2,1-3,2 m kalınlığındaki surlarla çevrelenmiştir.",
  "1,2 km uzunluğundaki sur duvarında 10 burç ve 2 kule tespit edilmiştir.",
  "Güney kulesi 19,2 m korunmuş yüksekliğiyle orijinalinde 21 m yüksekliğindeydi.",
  "Güney kulede yer altına doğru devam eden gizli bir geçit tespit edilmiştir.",
  "Arsenal yapısı 10,9x36,4 m boyutlarında iki bölümlü bir silah deposudur.",
  "Arsenal 1975 yılına kadar ayakta kalmış, o tarihte doğu duvarı yıkılmıştır.",
  "Askeri üst düzey yönetici konutu 9,6x12,4 m boyutlarında beş mekândan oluşmaktadır.",
  // Su sistemi
  "Yerleşimde 63 adet ana kayaya oyulmuş sarnıç yapısı tespit edilmiştir.",
  "Ana su deposu 11,2x22,5 m boyutlarında iki bölümlü tonozlu büyük sarnıçlardır.",
  "Su kanalının 616 m lik kısmı açığa çıkarılmıştır. Genişliği 51-68 cm, derinliği 48-61 cm dir.",
  // Mithras Tapınağı
  "Mithras Tapınağı Mithraeum 2017 yılında ortaya çıkarıldı.",
  "Tapınak dünya üzerindeki en iyi korunmuş Mithras tapınaklarından biridir.",
  "Mithras Hint-Pers kökenli bir tanrıdır.",
  "Zerzevan Mithraeum Roma sınırları içindeki en eski tapınaklardan biridir.",
  "Tapınak öncesinde 2016 yılında yeraltı yapısı tespit edildi. Boyutları 14x7 m, yüksekliği 4,3 m dir.",
  "Tapınak koridorunda çözümlenemeyen yazıtlar ve semboller bulunmaktadır.",
  "Boğa kurban törenlerinin tapınağın hemen dışındaki yapıda gerçekleştirildiği anlaşılmıştır.",
  // Kilise
  "Yerleşim merkezinde Roma Dönemi kaya mezarından dönüştürülmüş yeraltı kilisesi bulunmaktadır.",
  "Yeraltı kilisesi MS 2-3 yüzyılda mezar olarak kullanılmış, sonra Hristiyan ibadetine açılmıştır.",
  "Kilisenin önünde ana kayaya oyulmuş bir vaftiz havuzu bulunmaktadır.",
  "Büyük kamu kilisesi yerleşimin en yüksek güney noktasına MS 6 yüzyılda inşa edilmiştir.",
  "Kiliseden İstanbul Arkeoloji Müzesi ndeki bronz kova gelmektedir. Envanter no 852.",
  // Nekropol
  "Sur dışında üç tür mezar tespit edilmiştir: tonozlu mezarlar, klineli kaya mezarları ve lahit biçiminde kaya mezarları.",
  "Nekropol alanında iki adet tonozlu mezar tespit edilmiştir.",
  "Sunu çanakları 85-58 cm çapında 24-28 cm derinliğindedir ve birbirine 8,2 m mesafededir.",
  // Procopius
  "Antik yazar Procopius Justinianos döneminde Dara ve Amida arasındaki kalelerin yeniden inşa edildiğinden bahseder.",
  "Procopius yeniden inşa edilen kaleler arasında Zerzevan a karşılık gelen bir yerden bahsetmez.",
  // Sasani - Roma
  "Sasani hükümdarı II Şapur MS 359 da Amida antik yolunu kullanarak Amida yı ele geçirdi.",
  "Dara garnizonu Anastasios I tarafından MS 503-507 yıllarında inşa edildi.",
  "Zervan ya da Zurvan Pers inancında zaman tanrısıdır ve Mithras ile yakın ilişkisi vardır.",
};

int datasetBoyut = 52;

// ========== YARDIMCI FONKSİYONLAR ==========
bool kelimeVar(String cumle, String kelime) {
  cumle.toLowerCase();
  kelime.toLowerCase();
  return cumle.indexOf(kelime) != -1;
}

bool isTurkish(String s) {
  s.toLowerCase();
  if(s.indexOf("ç")!=-1 || s.indexOf("ğ")!=-1 || s.indexOf("ı")!=-1 ||
     s.indexOf("ö")!=-1 || s.indexOf("ş")!=-1 || s.indexOf("ü")!=-1) return true;
  return true; // Varsayılan Türkçe
}

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

// ========== KELİME ARAMA FONKSİYONU ==========
// Dataset içinde belirli bir kelimeyi arar, bulunan cümleleri döndürür
String kelimeAra(String arananKelime) {
  if(arananKelime.length() < 2) {
    return "Arama kelimesi en az 2 karakter olmalıdır.";
  }
  
  arananKelime.trim();
  String arananKucuk = arananKelime;
  arananKucuk.toLowerCase();
  
  String sonuclar = "";
  int bulunanSayisi = 0;
  
  for(int i = 0; i < datasetBoyut; i++) {
    String cumle = String(dataset[i]);
    String cumleLower = cumle;
    cumleLower.toLowerCase();
    
    if(cumleLower.indexOf(arananKucuk) != -1) {
      bulunanSayisi++;
      if(bulunanSayisi <= 5) { // En fazla 5 sonuç göster
        sonuclar += String(bulunanSayisi) + ". " + cumle + "\n";
      }
    }
  }
  
  if(bulunanSayisi == 0) {
    return "'" + arananKelime + "' kelimesi veritabanında bulunamadı. Farklı bir kelime deneyin.";
  }
  
  String cevap = "🔍 '" + arananKelime + "' için " + String(bulunanSayisi) + " sonuç bulundu:\n\n";
  cevap += sonuclar;
  if(bulunanSayisi > 5) {
    cevap += "... ve " + String(bulunanSayisi - 5) + " sonuç daha.";
  }
  return cevap;
}

// ========== ARAMA KOMUTU KONTROLÜ ==========
bool aramaKomutumu(String soru) {
  soru.toLowerCase();
  return (kelimeVar(soru, "ara ") || kelimeVar(soru, "search ") ||
          kelimeVar(soru, "bul ") || kelimeVar(soru, "içinde ara") ||
          kelimeVar(soru, "metinde ara") || soru.startsWith("ara:") ||
          soru.startsWith("search:"));
}

String aramaKelimesiCikar(String soru) {
  soru.trim();
  String lower = soru;
  lower.toLowerCase();
  
  // "ara: kelime" veya "search: kelime" formatı
  int iki = soru.indexOf(":");
  if(iki != -1) {
    String kelime = soru.substring(iki + 1);
    kelime.trim();
    return kelime;
  }
  
  // "ara kelime" veya "bul kelime" formatı
  int idx = lower.indexOf("ara ");
  if(idx != -1) return soru.substring(idx + 4);
  
  idx = lower.indexOf("bul ");
  if(idx != -1) return soru.substring(idx + 4);
  
  idx = lower.indexOf("search ");
  if(idx != -1) return soru.substring(idx + 7);
  
  return soru;
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

// ========== ANA CEVAP FONKSİYONU ==========
String getCevap(String soru) {
  soru.trim();
  String orijinalSoru = soru;
  
  // ===== ARAMA KOMUTU =====
  if(aramaKomutumu(soru)) {
    String kelime = aramaKelimesiCikar(soru);
    return kelimeAra(kelime);
  }
  
  // ===== ONAY KONTROLÜ =====
  if(onayMi(soru) && hafiza.takipSorusuVar && hafiza.bekleyenKonu != "") {
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
      
      if(konular[konuIdx].takipSayisi > 0 && random(0, 100) > 40) {
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
    // Dataset'te kelime araması yap
    String lower = soru;
    lower.toLowerCase();
    // Tek kelimelik sorgular için direkt dataset araması
    if(soru.indexOf(" ") == -1 && soru.length() >= 3) {
      String aramaS = kelimeAra(soru);
      if(aramaS.indexOf("bulunamadı") == -1) {
        return aramaS;
      }
    }
    
    if(hafiza.sonKonu != "" && hafiza.konusmaSayisi > 0) {
      return "Hmm, tam anlayamadım. 🤔 Şunları sorabilirsiniz: tarih, mimari, Mithras tapınağı, kilise, su sistemi, askeri yapı, konum, günümüz. Veya 'ara: kelime' yazarak bilgi arayabilirsiniz!";
    }
    return "Bu konuda yeterli bilgim yok. 🤔 Zerzevan Kalesi hakkında merak ettiklerinizi sorun, ya da 'ara: kelime' yazarak belirli bir terimi arayabilirsiniz!";
  }
  
  // ===== CEVAP OLUŞTURMA =====
  String cevap = "";
  cevap += konular[konuIdx].cevaplar[random(0, konular[konuIdx].cevapSayisi)];
  
  if(konular[konuIdx].takipSayisi > 0 && random(0, 100) > 30) {
    int takipIdx = random(0, konular[konuIdx].takipSayisi);
    cevap += "\n\n" + konular[konuIdx].takipSorulari[takipIdx];
    hafiza.takipSorusuVar = true;
    
    String takipSorusu = konular[konuIdx].takipSorulari[takipIdx];
    takipSorusu.toLowerCase();
    
    if(kelimeVar(takipSorusu, "mimari")) hafiza.bekleyenKonu = "mimari";
    else if(kelimeVar(takipSorusu, "tarih")) hafiza.bekleyenKonu = "tarih";
    else if(kelimeVar(takipSorusu, "mithras") || kelimeVar(takipSorusu, "tapınak")) hafiza.bekleyenKonu = "mithra";
    else if(kelimeVar(takipSorusu, "asker") || kelimeVar(takipSorusu, "ordu")) hafiza.bekleyenKonu = "askeri";
    else if(kelimeVar(takipSorusu, "konum") || kelimeVar(takipSorusu, "nerede")) hafiza.bekleyenKonu = "konum";
    else if(kelimeVar(takipSorusu, "günümüz") || kelimeVar(takipSorusu, "ziyaret")) hafiza.bekleyenKonu = "gunumuz";
    else if(kelimeVar(takipSorusu, "sasani")) hafiza.bekleyenKonu = "sasani";
    else if(kelimeVar(takipSorusu, "roma")) hafiza.bekleyenKonu = "roma";
    else if(kelimeVar(takipSorusu, "su") || kelimeVar(takipSorusu, "sarnıç")) hafiza.bekleyenKonu = "su";
    else if(kelimeVar(takipSorusu, "kilise")) hafiza.bekleyenKonu = "kilise";
    else hafiza.bekleyenKonu = konular[(konuIdx + 1) % konuSayisi].id;
  } else {
    hafiza.takipSorusuVar = false;
  }
  
  hafiza.sonKonu = konular[konuIdx].id;
  hafiza.sonSoru = orijinalSoru;
  hafiza.sonCevap = cevap;
  hafiza.konusmaSayisi++;
  hafiza.sonZaman = millis();
  
  return cevap;
}

// ========== HTML ARAYÜZÜ ==========
const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Zerzevan Kalesi - AI Rehber</title>
<style>
* { margin: 0; padding: 0; box-sizing: border-box; }
body {
  font-family: 'Segoe UI', system-ui, sans-serif;
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
  min-height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 10px;
}
.container {
  background: rgba(255,255,255,0.97);
  border-radius: 24px;
  box-shadow: 0 25px 80px rgba(0,0,0,0.5);
  max-width: 520px;
  width: 100%;
  overflow: hidden;
  animation: slideUp 0.6s ease-out;
}
@keyframes slideUp {
  from { opacity: 0; transform: translateY(30px); }
  to { opacity: 1; transform: translateY(0); }
}
.header {
  background: linear-gradient(135deg, #b8860b 0%, #8B0000 50%, #0f3460 100%);
  color: white;
  padding: 24px 20px;
  text-align: center;
  position: relative;
  overflow: hidden;
}
.header h1 { font-size: 1.7em; margin-bottom: 5px; text-shadow: 2px 2px 4px rgba(0,0,0,0.4); }
.header p { font-size: 0.9em; opacity: 0.9; }
.status {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  background: rgba(255,255,255,0.2);
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 0.8em;
  margin-top: 10px;
}
.status-dot { width: 8px; height: 8px; background: #00ff88; border-radius: 50%; animation: pulse 1.5s infinite; }
@keyframes pulse { 0%,100%{opacity:1;transform:scale(1)} 50%{opacity:0.5;transform:scale(1.2)} }

.search-bar {
  display: flex;
  gap: 8px;
  padding: 12px 15px;
  background: #fff3cd;
  border-bottom: 2px solid #ffc107;
}
.search-bar input {
  flex: 1;
  padding: 10px 14px;
  border: 2px solid #ffc107;
  border-radius: 20px;
  font-size: 14px;
  outline: none;
  background: white;
}
.search-bar input:focus { border-color: #ff8c00; }
.search-bar button {
  padding: 10px 18px;
  background: linear-gradient(135deg, #ff8c00, #b8860b);
  color: white;
  border: none;
  border-radius: 20px;
  font-size: 13px;
  font-weight: 700;
  cursor: pointer;
}

.quick-btns {
  display: flex;
  flex-wrap: wrap;
  gap: 7px;
  padding: 12px;
  background: #f8f9fa;
  justify-content: center;
  border-bottom: 1px solid #dee2e6;
}
.quick-btn {
  padding: 8px 14px;
  background: white;
  border: 2px solid #8B0000;
  color: #8B0000;
  border-radius: 20px;
  font-size: 12px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.3s;
}
.quick-btn:hover, .quick-btn:active { background: #8B0000; color: white; transform: scale(1.05); }

.chat {
  padding: 16px;
  max-height: 360px;
  min-height: 260px;
  overflow-y: auto;
  background: #f0f2f5;
}
.msg {
  margin-bottom: 14px;
  animation: fadeIn 0.4s ease-out;
  display: flex;
  align-items: flex-start;
  gap: 8px;
}
@keyframes fadeIn { from{opacity:0;transform:translateY(10px)} to{opacity:1;transform:translateY(0)} }
.msg.user { flex-direction: row-reverse; }
.avatar { width:34px;height:34px;border-radius:50%;display:flex;align-items:center;justify-content:center;font-size:17px;flex-shrink:0; }
.msg.bot .avatar { background: linear-gradient(135deg, #b8860b, #8B0000); }
.msg.user .avatar { background: linear-gradient(135deg, #667eea, #764ba2); }
.bubble {
  padding: 12px 16px;
  border-radius: 18px;
  max-width: 82%;
  line-height: 1.6;
  font-size: 13.5px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.08);
  white-space: pre-line;
}
.msg.user .bubble { background: linear-gradient(135deg, #667eea, #764ba2); color: white; border-bottom-right-radius: 4px; }
.msg.bot .bubble { background: white; color: #1a1a2e; border-bottom-left-radius: 4px; }
.msg.bot .bubble.search-result { background: #fff8e1; border-left: 4px solid #ffc107; }

.typing { display:none; padding:12px 16px; background:white; border-radius:18px; width:fit-content; margin-left:42px; margin-bottom:14px; }
.typing.show { display:block; }
.typing span { display:inline-block;width:8px;height:8px;border-radius:50%;background:#8B0000;margin:0 2px;animation:bounce 1.4s infinite ease-in-out; }
.typing span:nth-child(2){animation-delay:.2s}
.typing span:nth-child(3){animation-delay:.4s}
@keyframes bounce{0%,60%,100%{transform:translateY(0)}30%{transform:translateY(-8px)}}

.input-area {
  padding: 16px;
  background: white;
  display: flex;
  gap: 8px;
  border-top: 1px solid #e9ecef;
}
input[type=text] {
  flex: 1;
  padding: 12px 16px;
  border: 2px solid #e9ecef;
  border-radius: 25px;
  font-size: 14px;
  outline: none;
  transition: all 0.3s;
}
input[type=text]:focus { border-color: #8B0000; box-shadow: 0 0 0 3px rgba(139,0,0,0.1); }
.send-btn {
  padding: 12px 22px;
  background: linear-gradient(135deg, #8B0000, #0f3460);
  color: white;
  border: none;
  border-radius: 25px;
  font-size: 14px;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.3s;
}
.send-btn:hover { transform: translateY(-2px); box-shadow: 0 6px 20px rgba(139,0,0,0.4); }

.footer {
  background: #1a1a2e;
  color: rgba(255,255,255,0.7);
  padding: 10px;
  text-align: center;
  font-size: 11px;
}
.footer span { color: #b8860b; }
</style>
</head>
<body>
<div class="container">
  <div class="header">
    <h1>Zerzevan Kalesi</h1>
    <p>Yapay Zeka Destekli Dijital Rehber — Doç. Dr. Aytaç Coşkun Araştırmaları</p>
    <div class="status">
      <span class="status-dot"></span>
      Akademik Veritabanı Aktif
    </div>
  </div>

  <div class="search-bar">
    <input type="text" id="searchInput" placeholder="🔍 Metinde kelime arayın... (örn: sarnıç, Mithras, UNESCO)">
    <button onclick="doSearch()">Ara</button>
  </div>

  <div class="quick-btns">
    <button class="quick-btn" onclick="ask('Tarihi nedir?')">📜 Tarih</button>
    <button class="quick-btn" onclick="ask('Mithras tapınağı nedir?')">🔮 Mithras</button>
    <button class="quick-btn" onclick="ask('Mimarisi nasıl?')">🏛️ Mimari</button>
    <button class="quick-btn" onclick="ask('Nerede bulunuyor?')">📍 Konum</button>
    <button class="quick-btn" onclick="ask('Kilise hakkında bilgi ver')">⛪ Kilise</button>
    <button class="quick-btn" onclick="ask('Su sistemi nasıl?')">💧 Su</button>
    <button class="quick-btn" onclick="ask('Kazılar ve günümüz')">🌍 Günümüz</button>
    <button class="quick-btn" onclick="ask('Askeri yapısı nedir?')">⚔️ Askeri</button>
  </div>

  <div class="chat" id="chat">
    <div class="msg bot">
      <div class="avatar">🏛️</div>
      <div class="bubble">Hoş geldiniz! 🏰 Ben Zerzevan Kalesi'nin akademik veritabanına dayanan dijital rehberiyim.

Doç. Dr. Aytaç Coşkun'un araştırmalarından derlenen bilgilerle Asur'dan Bizans'a uzanan 2600 yıllık tarihi keşfedin!

🔍 Kelime Araması: Üstteki arama çubuğunu kullanarak metinlerde belirli terimleri arayabilirsiniz (örn: "sarnıç", "Mithras", "Justinianos").</div>
    </div>
  </div>

  <div id="typing" class="typing">
    <span></span><span></span><span></span>
  </div>

  <div class="input-area">
    <input type="text" id="q" placeholder="Sorunuzu yazın..." onkeypress="if(event.key=='Enter')sendQ()">
    <button class="send-btn" onclick="sendQ()">Gönder</button>
  </div>

  <div class="footer">
    <span>Kaynak:</span> Coşkun, A. — Zerzevan Kalesi Kazılarının Dünü ve Bugünü | <span>UNESCO Geçici Liste 2020</span>
  </div>
</div>

<script>
function ask(q) {
  document.getElementById('q').value = q;
  sendQ();
}

function doSearch() {
  let term = document.getElementById('searchInput').value.trim();
  if(!term) return;
  document.getElementById('q').value = 'ara: ' + term;
  sendQ();
  document.getElementById('searchInput').value = '';
}

document.getElementById('searchInput').addEventListener('keypress', function(e) {
  if(e.key === 'Enter') doSearch();
});

async function sendQ() {
  let q = document.getElementById('q').value.trim();
  if (!q) return;

  addMsg(q, 'user', false);
  document.getElementById('q').value = '';

  document.getElementById('typing').classList.add('show');
  scrollChat();

  try {
    let r = await fetch('/ask?q=' + encodeURIComponent(q));
    let d = await r.json();
    document.getElementById('typing').classList.remove('show');
    let isSearch = q.toLowerCase().startsWith('ara:') || q.toLowerCase().startsWith('search:');
    setTimeout(() => addMsg(d.answer, 'bot', isSearch), 200);
  } catch(e) {
    document.getElementById('typing').classList.remove('show');
    addMsg('Bağlantı hatası! Tekrar deneyin.', 'bot', false);
  }
}

function addMsg(txt, who, isSearch) {
  let c = document.getElementById('chat');
  let m = document.createElement('div');
  m.className = 'msg ' + who;

  let av = document.createElement('div');
  av.className = 'avatar';
  av.textContent = who === 'bot' ? '🏛️' : '👤';

  let b = document.createElement('div');
  b.className = 'bubble' + (isSearch && who === 'bot' ? ' search-result' : '');
  b.textContent = txt;

  m.appendChild(av);
  m.appendChild(b);
  c.appendChild(m);
  scrollChat();
}

function scrollChat() {
  let c = document.getElementById('chat');
  c.scrollTop = c.scrollHeight;
}
</script>
</body>
</html>
)rawliteral";

// ========== CAPTIVE PORTAL ==========
const char captiveHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="0;url=http://192.168.4.1/">
</head>
<body><script>window.location.href='http://192.168.4.1/';</script></body>
</html>
)rawliteral";

// ========== HTTP HANDLERS ==========
void handleRoot() {
  server.send_P(200, "text/html", html);
}

void handleAsk() {
  if (server.hasArg("q")) {
    String cevap = getCevap(server.arg("q"));
    cevap.replace("\"", "\\\"");
    cevap.replace("\n", "\\n");
    server.send(200, "application/json", "{\"answer\":\"" + cevap + "\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"Soru yok\"}");
  }
}

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
  Serial.println("╔══════════════════════════════════════════════════════════╗");
  Serial.println("║   ZERZEVAN KALESİ - AI CHATBOT v5.0 (DOCX Dataset)      ║");
  Serial.println("╠══════════════════════════════════════════════════════════╣");
  Serial.println("║  ✓ Docx Dataset Entegrasyonu (52 akademik veri noktası)  ║");
  Serial.println("║  ✓ Kelime Arama Özelliği (ara: kelime)                   ║");
  Serial.println("║  ✓ 13 Konu Kategorisi (Kilise, Nekropol dahil)           ║");
  Serial.println("║  ✓ Bağlam Hafızası ve Takip Soruları                     ║");
  Serial.println("║  ✓ Captive Portal (Otomatik Sayfa Açma)                  ║");
  Serial.println("╚══════════════════════════════════════════════════════════╝\n");

  randomSeed(analogRead(0) + millis());

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  delay(100);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("[+] IP Adresi: ");
  Serial.println(IP);

  dnsServer.start(DNS_PORT, "*", IP);

  server.on("/", handleRoot);
  server.on("/ask", handleAsk);
  server.on("/generate_204", handleCaptive);
  server.on("/gen_204", handleCaptive);
  server.on("/hotspot-detect.html", handleCaptive);
  server.on("/library/test/success.html", handleCaptive);
  server.on("/ncsi.txt", handleCaptive);
  server.on("/connecttest.txt", handleCaptive);
  server.on("/redirect", handleCaptive);
  server.on("/success.txt", handleCaptive);
  server.on("/canonical.html", handleCaptive);
  server.on("/kindle-wifi/wifistub.html", handleCaptive);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("[+] Web Server başlatıldı!\n");

  Serial.println(">>> YENİ ÖZELLİKLER v5.0:");
  Serial.println("────────────────────────────────────────");
  Serial.println("🔍 KELİME ARAMA:");
  Serial.println("   • 'ara: sarnıç'  → Dataset içinde arar");
  Serial.println("   • 'ara: Mithras' → İlgili cümleleri listeler");
  Serial.println("   • 'ara: 2014'    → Yıla göre arama yapar");
  Serial.println("   • 'ara: UNESCO'  → Konu araması");
  Serial.println("");
  Serial.println("📚 DATASET: Doç. Dr. Aytaç Coşkun makalesi");
  Serial.println("   • 52 akademik veri noktası");
  Serial.println("   • Tarih, mimari, Mithras, kilise, nekropol");
  Serial.println("   • Su sistemi, askeri yapı, konum bilgileri");
  Serial.println("────────────────────────────────────────\n");

  Serial.println(">>> KULLANIM:");
  Serial.println("1. WiFi: 'Zerzevan Kalesi Yapay Zeka'");
  Serial.println("2. Sayfa otomatik açılır");
  Serial.println("3. Açılmazsa: http://192.168.4.1\n");
  Serial.println("════════════════════════════════════════\n");
}

// ========== LOOP ==========
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
