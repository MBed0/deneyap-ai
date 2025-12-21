

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// ══════════════════════════════════════════════════════════════════════════════
// YAPILANDIRMA
// ══════════════════════════════════════════════════════════════════════════════

const char* AP_SSID = "Zerzevan Kalesi Rehber";
const char* AP_PASS = "";  // Açık ağ (şifresiz)

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

DNSServer dnsServer;
WebServer server(80);

// ══════════════════════════════════════════════════════════════════════════════
// DİL DESTEĞİ
// ══════════════════════════════════════════════════════════════════════════════

enum Language { TR, EN };
Language currentLang = TR;
String lastContext = "";
int contextCount = 0;

// ══════════════════════════════════════════════════════════════════════════════
// İÇERİK VERİTABANI - TÜRKÇE
// ══════════════════════════════════════════════════════════════════════════════

// Genel Bilgiler
const char* TR_GENEL[] PROGMEM = {
  "Zerzevan Kalesi, Diyarbakır'ın Çınar ilçesinde, MS 3. yüzyılda Roma İmparatorluğu tarafından inşa edilmiş stratejik bir askeri üstür. 🏰 Doğu sınırının en önemli savunma noktalarından biriydi!",
  "Hoş geldiniz! Zerzevan, Roma'nın Mezopotamya'daki en büyük askeri garnizonlarından biridir. 21 hektarlık alanda surlar, tapınaklar, yeraltı yapıları ve askeri binalar bulunur.",
  "Zerzevan Kalesi hakkında her şeyi öğrenebilirsiniz! Roma askeri mimarisi, Mithras kültü, su sistemleri, günlük yaşam... Ne merak ediyorsunuz? 🤔",
  "Burası antik dünyanın en gizemli yerlerinden biri! Roma askerleri burada Mithras'a tapıyor, Sasanilere karşı savaşıyor ve İpek Yolu'nu koruyorlardı."
};

// Tarih
const char* TR_TARIH[] PROGMEM = {
  "📜 Zerzevan'ın Tarihi Serüveni:\n\n• MS 3. yüzyıl: Roma tarafından inşa edildi\n• MS 3-4. yüzyıl: Sasani saldırılarına karşı ana savunma hattı\n• MS 5-6. yüzyıl: Bizans döneminde kullanım devam etti\n• MS 7. yüzyıl: Terk edildi\n• 2014: Arkeolojik kazılar başladı\n• 2020: UNESCO Dünya Mirası Geçici Listesi'ne alındı",
  "Roma İmparatoru Diocletianus döneminde (MS 284-305) kale stratejik öneme kavuştu. Sasani İmparatorluğu'na karşı 'Limes' savunma hattının kritik noktasıydı.",
  "İlginç bir detay! 🎯 Zerzevan, Roma'nın 'Strata Diocletiana' adlı askeri yol ağının üzerindeydi. Bu yol Suriye'den Karadeniz'e kadar uzanıyordu.",
  "Kale MS 7. yüzyılda İslam fetihlerinden sonra terk edildi. Neredeyse 1400 yıl toprak altında kaldıktan sonra 2014'te kazılar başladı!"
};

// Mithras Tapınağı
const char* TR_MITHRAS[] PROGMEM = {
  "🔥 Mithras Tapınağı - Dünyanın En İyi Korunmuş Örneklerinden Biri!\n\nMithras, Pers kökenli güneş ve ışık tanrısıdır. Roma askerleri arasında çok popülerdi çünkü:\n• Sadakat ve kardeşlik vurguluyordu\n• Cesaret ve güç sembolüydü\n• Gizli ritüelleri vardı (sadece erkekler katılabilirdi)",
  "Tapınak yeraltında, kayalara oyulmuş bir mağarada! İçeride Mithras'ın boğa kesme sahnesi (tauroctony) kabartması var. Bu sahne evrenin yaratılışını simgeler.",
  "Biliyor musunuz? 🌟 Mithras kültü Hristiyanlıkla aynı dönemde yayıldı. 25 Aralık Mithras'ın doğum günüydü - sonradan Noel olarak benimsendi!",
  "Mithras'ın 7 inisiyasyon derecesi vardı: Kuzgun, Gelin, Asker, Aslan, Pers, Güneş Habercisi ve Pater (Baba). Her derece için özel ritüeller yapılırdı.",
  "Tapınaktaki yıldız sembolleri astronomiyle bağlantılı! Boğa burcu, Akrep burcu ve diğer takımyıldızlar tasvir edilmiş. Roma askerleri hem savaşçı hem de gökbilimciydi! ⭐"
};

// Mimari ve Yapılar
const char* TR_MIMARI[] PROGMEM = {
  "🏗️ Zerzevan'ın Mimari Harikaları:\n\n• 1200 metre sur duvarı (yükseklik: 12-15 metre)\n• 21 kule\n• Kaya mezarları\n• Yeraltı sığınakları ve tüneller\n• Sarnıçlar ve su kanalları\n• Kilise kalıntıları\n• İdari binalar ve kışlalar",
  "Surlar hem savunma hem de gösterişti! 💪 3 metre kalınlığında, düzgün kesilmiş taşlardan yapılmış. Düşmana 'buraya girmeye çalışma' mesajı veriyordu.",
  "Kuleler dikdörtgen planlı ve surlardan dışarı çıkıntılı. Bu tasarım sayesinde düşman her açıdan ok yağmuruna tutulabiliyordu - taktik deha!",
  "Kaya mezarları zengin Roma vatandaşları için yapılmış. İçlerinde lahitler, freskler ve yazıtlar bulundu. Ölümden sonra yaşam inancını yansıtıyorlar."
};

// Su Sistemleri
const char* TR_SU[] PROGMEM = {
  "💧 Zerzevan'ın Su Mühendisliği - Roma Dehasının Kanıtı!\n\nKale, sofistike bir su sistemine sahipti:\n• Kaynak sularını toplayan kanallar\n• Yeraltı sarnıçları (binlerce litre kapasiteli)\n• Yağmur suyu toplama sistemleri\n• Hamam ve çeşmeler için dağıtım ağı",
  "Sarnıçlar kayaya oyulmuş devasa hazneler! Kuşatma zamanlarında aylarca su ihtiyacını karşılayabiliyordu. Su güvenliği = hayatta kalma! 🏊",
  "İlginç detay: Roma mühendisleri su kanallarının eğimini milimetrik hesapladı. Yerçekimi gücüyle su, kalenin her noktasına ulaşıyordu - pompa yok, elektrik yok!",
  "Hamamlar sadece temizlik için değildi - sosyal merkezdiler! Askerler burada dedikodu yapıyor, ticaret konuşuyor, politik tartışmalara giriyordu. Antik sosyal medya! 🛁"
};

// Askeri Yaşam
const char* TR_ASKERI[] PROGMEM = {
  "⚔️ Roma Askeri Yaşamı Zerzevan'da:\n\n• Sabah: Şafakta kalkış, eğitim\n• Öğle: Hafif yemek, görev\n• Akşam: Ana yemek, dinlenme\n• Gece: Nöbet rotasyonu\n\nAskerler 25 yıl hizmet ederdi!",
  "Bir Roma askeri (legioner) günde yaklaşık 30 km yürüyebilirdi - tam teçhizatla! 🏃 Gladius (kısa kılıç), pilum (mızrak), scutum (kalkan) ve 20 kg zırh taşırlardı.",
  "Askerler maaşlarının bir kısmını zorunlu tasarrufa yatırırdı. 25 yıl sonra emekli olduklarında toprak ve vatandaşlık hakkı kazanırlardı. Erken emeklilik sistemi!",
  "Zerzevan'daki kışlalar 8 kişilik 'contubernium' birimleri için tasarlanmış. Bu 8 asker birlikte yaşar, savaşır ve yerdi. Kardeşlikten öte bir bağdı!",
  "Disiplin çok sert! Uyuyan nöbetçi idam edilirdi. Kaçaklar yakalanırsa kırbaçlanır veya köle yapılırdı. Ama sadık askerler terfi ve ödül alırdı. ⚖️"
};

// Sasani İmparatorluğu
const char* TR_SASANI[] PROGMEM = {
  "🛡️ Sasani Tehdidi:\n\nSasani İmparatorluğu (MS 224-651) Roma'nın en güçlü düşmanıydı. İran merkezli bu imparatorluk sürekli Mezopotamya için savaştı.\n\nZerzevan tam bu sınırda, ateş hattındaydı!",
  "MS 260'ta Sasani Kralı I. Şapur, Roma İmparatoru Valerianus'u esir aldı - tarihte eşi görülmemiş bir olay! Bu utançtan sonra Roma sınır savunmasını güçlendirdi.",
  "Sasaniler Zerdüşt dinine bağlıydı - ateşe taparlardı. İlginç şekilde, hem onlar hem de Romalı Mithras müritleri 'ışık' ve 'ateş' sembolizmi kullanıyordu! 🔥",
  "Sasani ordusu ağır zırhlı süvarilerle (cataphract) ünlüydü. Roma buna karşı Zerzevan gibi güçlü kaleler inşa etti - mobil düşmana sabit savunma!"
};

// Günlük Yaşam
const char* TR_YASAM[] PROGMEM = {
  "🍷 Zerzevan'da Günlük Yaşam:\n\n• Sabah kahvaltısı: Ekmek, zeytin, peynir\n• Öğle: Bulamaç (puls), sebze\n• Akşam: Et (nadir), balık, şarap\n\nAskerler kendi yemeklerini pişirirdi!",
  "Eğlence de vardı! 🎲 Zar oyunları, güreş, atletizm yarışları... Bayramlarda özel yemekler ve şarap dağıtılırdı. Gladyatör dövüşleri de izlenirdi.",
  "Mektup yazımı çok yaygındı. Askerler aileleriyle yazışır, para gönderirdi. Vindolanda tabletleri gibi örnekler duygusal mesajlar içeriyor: 'Çorap gönder, burada ayaklarım donuyor!'",
  "Kadınlar resmi olarak kalede yaşayamazdı ama yakın köylerde asker eşleri ve çocukları bulunurdu. Emeklilikle resmi evlilik hakkı kazanılırdı. 💑"
};

// İpek Yolu
const char* TR_IPEK[] PROGMEM = {
  "🐪 İpek Yolu ve Zerzevan:\n\nZerzevan, Çin'den Roma'ya uzanan efsanevi İpek Yolu'nun güvenliğini sağlayan kalelerden biriydi. Ticaret kervanları burada konaklardı.",
  "İpek Yolu sadece ipek taşımıyordu! Baharat, porselen, cam, değerli taşlar, egzotik hayvanlar ve en önemlisi FİKİRLER taşıyordu. Kültürel alışveriş! 🌍",
  "Kervanlar deve ve katırlarla yolculuk ederdi. Tek bir kervan 100'den fazla hayvan ve 50 tüccardan oluşabilirdi. Yol tehlikeliydi - eşkıyalar, çöl, savaşlar...",
  "Zerzevan'da Çin'den gelen ipek parçaları, Hint baharatları ve Pers mücevherleri bulundu. Bu küçük kale dünya ticaretinin merkezindeydi! 💎"
};

// Arkeoloji
const char* TR_ARKEOLOJI[] PROGMEM = {
  "🔍 Arkeolojik Kazılar:\n\n2014'ten beri Dicle Üniversitesi öncülüğünde kazılar sürüyor. Her yıl yeni keşifler yapılıyor!\n\nBulunan önemli eserler:\n• Mithras tapınağı\n• Altın ve gümüş takılar\n• Seramikler ve cam eşyalar\n• Yazılı tabletler\n• Silah ve zırhlar",
  "Kazı teknikleri çok hassas! 🖌️ Arkeologlar fırça, spatula ve hatta diş hekimi aletleri kullanıyor. Tek bir sikke için günlerce çalışılabiliyor.",
  "3D tarama ve drone teknolojisi kullanılıyor. Yeraltı yapıları radar ile tespit ediliyor - henüz kazılmamış gizli odalar olabilir! 🛸",
  "Bulunan eserler Diyarbakır Arkeoloji Müzesi'nde sergileniyor. Bazı özel parçalar Ankara ve İstanbul müzelerinde de görülebilir."
};

// UNESCO
const char* TR_UNESCO[] PROGMEM = {
  "🌟 UNESCO Dünya Mirası:\n\nZerzevan Kalesi 2020'de UNESCO Dünya Mirası Geçici Listesi'ne alındı. Tam listeye alınma süreci devam ediyor.\n\nSeçilme nedenleri:\n• Eşsiz Mithras tapınağı\n• Korunmuş Roma askeri mimarisi\n• Tarihsel önemi",
  "UNESCO listesine girerse dünya çapında tanınırlık kazanacak! Göbeklitepe ve Efes gibi diğer Türk sitleriyle birlikte küresel miras olacak. 🏆",
  "Koruma çalışmaları hız kazandı. Surlar restore ediliyor, ziyaretçi altyapısı geliştiriliyor. Sürdürülebilir turizm planlanıyor.",
  "Siz de Zerzevan'ı ziyaret ederek bu mirası destekleyebilirsiniz! Her bilet koruma çalışmalarına katkı sağlıyor. 🎟️"
};

// Ziyaret Bilgileri
const char* TR_ZIYARET[] PROGMEM = {
  "📍 Ziyaret Bilgileri:\n\n• Konum: Diyarbakır, Çınar ilçesi, Demirölçek köyü (şehir merkezine 13 km)\n• Açık: Her gün 08:00-19:00 (yaz), 08:00-17:00 (kış)\n• Giriş: Müzekart geçerli\n• Ulaşım: Diyarbakır'dan dolmuş veya taksi",
  "Ziyaret önerileri:\n🥾 Rahat yürüyüş ayakkabısı giyin\n🧢 Şapka ve güneş kremi alın\n💧 Su getirin\n📸 Fotoğraf makinesi unutmayın\n⏱️ En az 2 saat ayırın",
  "En iyi ziyaret zamanı: İlkbahar (Nisan-Mayıs) ve sonbahar (Eylül-Ekim). Yaz ayları çok sıcak olabilir! ☀️",
  "Rehberli turlar mevcut! Yerel rehberler size bilinmeyen hikayeleri anlatır. Önceden rezervasyon yapmanız önerilir. 📞"
};

// Efsaneler ve Gizemler
const char* TR_EFSANE[] PROGMEM = {
  "👻 Zerzevan'ın Gizemleri:\n\nYeraltı tünellerinin nereye gittiği hâlâ bilinmiyor! Bazı tüneller yüzlerce metre uzanıyor ve henüz keşfedilmemiş odalara açılıyor olabilir.",
  "Yerel efsaneye göre kale altında Roma hazinesi gömülü! 💰 Sasani kuşatması sırasında askerler altınları gizlemiş ve haritayı yok etmiş. Gerçek mi?",
  "Mithras tapınağında yapılan törenlerin tam detayları hâlâ gizemini koruyor. Yazılı kaynak yok çünkü sırları açıklamak yasaktı! 🤫",
  "Bazı ziyaretçiler gece garip ışıklar gördüğünü iddia ediyor. Bilim insanları bunun fosforlu mineraller olduğunu söylüyor ama efsaneler devam ediyor... 👀"
};

// Selamlaşmalar ve Genel
const char* TR_SELAM[] PROGMEM = {
  "Merhaba! 👋 Zerzevan Kalesi Dijital Rehberine hoş geldiniz! Size Roma döneminin bu muhteşem askeri üssünü tanıtmaktan mutluluk duyarım. Ne öğrenmek istersiniz?",
  "Selamlar! 🏰 Ben Zerzevan'ın AI rehberiyim. Tarih, Mithras kültü, mimari, günlük yaşam... Aklınıza ne gelirse sorun!",
  "Hoşbulduk! Zerzevan'ın 1800 yıllık hikayesini keşfetmeye hazır mısınız? 🎭 Hangi konuyla başlayalım?",
  "Selam! Bugün Zerzevan'da sanal bir yolculuğa çıkacağız. Antik Roma dünyasına adım atmaya hazır mısınız? ⚔️"
};

// Devam Soruları
const char* TR_DEVAM[] PROGMEM = {
  "Bu konu hakkında daha fazla bilgi ister misiniz? Örneğin detayları, ilginç hikayeleri veya başka bir konuya geçebiliriz! 🤔",
  "Başka ne merak ediyorsunuz? Tarih, mimari, günlük yaşam, Mithras... Hepsini anlatabilirim!",
  "Sorularınızı bekliyorum! 💬 Zerzevan hakkında bilmek istediğiniz her şeyi sorun.",
  "Devam edelim mi? Belki Mithras tapınağını, su sistemlerini veya asker yaşamını merak ediyorsunuzdur?"
};

// Bilinmeyen Sorular
const char* TR_BILINMEYEN[] PROGMEM = {
  "Hmm, bu soruyu tam anlayamadım 🤔 Zerzevan Kalesi, Roma tarihi, Mithras kültü veya arkeoloji hakkında soru sorabilirsiniz!",
  "Bu konuda bilgim yok ama Zerzevan hakkında her şeyi biliyorum! Tarih, mimari, günlük yaşam... Ne öğrenmek istersiniz?",
  "Sorunuzu farklı şekilde sorabilir misiniz? Zerzevan Kalesi ile ilgili konularda size yardımcı olabilirim! 🏰",
  "Bu benim uzmanlık alanım dışında kaldı! 😅 Ama Zerzevan'ın Roma dönemi, Mithras tapınağı veya arkeolojik keşifler hakkında sormak ister misiniz?"
};

// Vedalaşma
const char* TR_VEDA[] PROGMEM = {
  "Görüşmek üzere! 👋 Zerzevan'ı ziyaret etmeyi unutmayın. Tarihi yerinde yaşamak bambaşka!",
  "Hoşça kalın! 🏰 Umarım Zerzevan hakkında yeni şeyler öğrenmişsinizdir. Tekrar beklerim!",
  "Güle güle! Roma askerlerinin selamıyla: AVE! Sağlıcakla kalın! ⚔️",
  "İyi günler! Zerzevan sizi bekliyor. Gerçek ziyarette görüşmek dileğiyle! 🌟"
};

// ══════════════════════════════════════════════════════════════════════════════
// İÇERİK VERİTABANI - İNGİLİZCE
// ══════════════════════════════════════════════════════════════════════════════

// General Info
const char* EN_GENEL[] PROGMEM = {
  "Zerzevan Castle is a strategic Roman military outpost built in the 3rd century AD, located in Çınar district of Diyarbakır. 🏰 It was one of the most important defense points on the Eastern frontier!",
  "Welcome! Zerzevan is one of Rome's largest military garrisons in Mesopotamia. The 21-hectare site includes walls, temples, underground structures, and military buildings.",
  "You can learn everything about Zerzevan Castle! Roman military architecture, Mithras cult, water systems, daily life... What are you curious about? 🤔",
  "This is one of the most mysterious places of the ancient world! Roman soldiers worshipped Mithras here, fought against Sasanians, and protected the Silk Road."
};

// History
const char* EN_TARIH[] PROGMEM = {
  "📜 Zerzevan's Historical Journey:\n\n• 3rd century AD: Built by Romans\n• 3rd-4th century: Main defense line against Sasanian attacks\n• 5th-6th century: Continued use during Byzantine period\n• 7th century: Abandoned\n• 2014: Archaeological excavations began\n• 2020: Added to UNESCO World Heritage Tentative List",
  "The castle gained strategic importance during Emperor Diocletian's reign (284-305 AD). It was a critical point of the 'Limes' defense line against the Sasanian Empire.",
  "Interesting fact! 🎯 Zerzevan was on Rome's military road network called 'Strata Diocletiana'. This road extended from Syria to the Black Sea.",
  "The castle was abandoned after the Islamic conquests in the 7th century. After nearly 1400 years underground, excavations began in 2014!"
};

// Mithras Temple
const char* EN_MITHRAS[] PROGMEM = {
  "🔥 Mithras Temple - One of the Best Preserved Examples in the World!\n\nMithras is a Persian god of sun and light. It was very popular among Roman soldiers because:\n• Emphasized loyalty and brotherhood\n• Symbol of courage and strength\n• Had secret rituals (only men could participate)",
  "The temple is underground, carved into rocks! Inside is a relief of Mithras slaying a bull (tauroctony). This scene symbolizes the creation of the universe.",
  "Did you know? 🌟 The Mithras cult spread during the same period as Christianity. December 25th was Mithras's birthday - later adopted as Christmas!",
  "Mithras had 7 initiation degrees: Raven, Bride, Soldier, Lion, Persian, Sun Runner, and Pater (Father). Special rituals were performed for each degree.",
  "The star symbols in the temple are connected to astronomy! Taurus, Scorpio and other constellations are depicted. Roman soldiers were both warriors and astronomers! ⭐"
};

// Architecture
const char* EN_MIMARI[] PROGMEM = {
  "🏗️ Zerzevan's Architectural Wonders:\n\n• 1200 meters of walls (height: 12-15 meters)\n• 21 towers\n• Rock tombs\n• Underground shelters and tunnels\n• Cisterns and water channels\n• Church remains\n• Administrative buildings and barracks",
  "The walls were both defense and display! 💪 3 meters thick, made of precisely cut stones. Sending the message 'don't even try to enter' to enemies.",
  "Towers are rectangular and protrude from the walls. This design allowed enemies to be showered with arrows from every angle - tactical genius!",
  "Rock tombs were built for wealthy Roman citizens. Sarcophagi, frescoes, and inscriptions were found inside. They reflect belief in life after death."
};

// Water Systems
const char* EN_SU[] PROGMEM = {
  "💧 Zerzevan's Water Engineering - Proof of Roman Genius!\n\nThe castle had a sophisticated water system:\n• Channels collecting spring water\n• Underground cisterns (thousands of liters capacity)\n• Rainwater collection systems\n• Distribution network for baths and fountains",
  "Cisterns are huge reservoirs carved into rock! During sieges, they could supply water for months. Water security = survival! 🏊",
  "Interesting detail: Roman engineers calculated channel slopes to the millimeter. Water reached every point of the castle by gravity - no pumps, no electricity!",
  "Baths weren't just for cleanliness - they were social centers! Soldiers gossiped, discussed trade, engaged in political debates here. Ancient social media! 🛁"
};

// Military Life
const char* EN_ASKERI[] PROGMEM = {
  "⚔️ Roman Military Life at Zerzevan:\n\n• Morning: Wake at dawn, training\n• Noon: Light meal, duty\n• Evening: Main meal, rest\n• Night: Guard rotation\n\nSoldiers served for 25 years!",
  "A Roman soldier (legionary) could march about 30 km per day - fully equipped! 🏃 They carried gladius (short sword), pilum (javelin), scutum (shield), and 20 kg of armor.",
  "Soldiers had to put part of their salary into forced savings. After 25 years of retirement, they earned land and citizenship rights. Early retirement system!",
  "Barracks at Zerzevan were designed for 8-person 'contubernium' units. These 8 soldiers lived, fought, and ate together. Beyond brotherhood!",
  "Discipline was very strict! Sleeping guards were executed. Deserters were whipped or enslaved if caught. But loyal soldiers received promotions and rewards. ⚖️"
};

// Greetings
const char* EN_SELAM[] PROGMEM = {
  "Hello! 👋 Welcome to Zerzevan Castle Digital Guide! I'm delighted to introduce you to this magnificent Roman military outpost. What would you like to learn?",
  "Greetings! 🏰 I'm Zerzevan's AI guide. History, Mithras cult, architecture, daily life... Ask me anything!",
  "Welcome! Are you ready to explore Zerzevan's 1800-year-old story? 🎭 Which topic shall we start with?",
  "Hi! Today we'll take a virtual journey through Zerzevan. Ready to step into the ancient Roman world? ⚔️"
};

// Unknown
const char* EN_BILINMEYEN[] PROGMEM = {
  "Hmm, I didn't quite understand that question 🤔 You can ask about Zerzevan Castle, Roman history, Mithras cult, or archaeology!",
  "I don't have information on this, but I know everything about Zerzevan! History, architecture, daily life... What would you like to learn?",
  "Could you rephrase your question? I can help you with topics related to Zerzevan Castle! 🏰",
  "That's outside my expertise! 😅 But would you like to ask about Zerzevan's Roman period, Mithras temple, or archaeological discoveries?"
};

// Farewell
const char* EN_VEDA[] PROGMEM = {
  "See you! 👋 Don't forget to visit Zerzevan. Experiencing history on site is something else!",
  "Goodbye! 🏰 I hope you've learned new things about Zerzevan. Come back anytime!",
  "Farewell! With the salute of Roman soldiers: AVE! Stay well! ⚔️",
  "Have a great day! Zerzevan awaits you. Hope to see you on a real visit! 🌟"
};

// Visit Info
const char* EN_ZIYARET[] PROGMEM = {
  "📍 Visit Information:\n\n• Location: Diyarbakır, Çınar district, Demirölçek village (13 km from city center)\n• Open: Daily 08:00-19:00 (summer), 08:00-17:00 (winter)\n• Entry: Museum card valid\n• Transport: Minibus or taxi from Diyarbakır",
  "Visit tips:\n🥾 Wear comfortable walking shoes\n🧢 Bring hat and sunscreen\n💧 Carry water\n📸 Don't forget your camera\n⏱️ Allow at least 2 hours",
  "Best time to visit: Spring (April-May) and autumn (September-October). Summer months can be very hot! ☀️",
  "Guided tours available! Local guides will tell you unknown stories. Advance booking recommended. 📞"
};

// ══════════════════════════════════════════════════════════════════════════════
// ANAHTAR KELİME EŞLEŞTİRME
// ══════════════════════════════════════════════════════════════════════════════

bool containsAny(String text, const char* keywords[], int count) {
  text.toLowerCase();
  for (int i = 0; i < count; i++) {
    if (text.indexOf(keywords[i]) != -1) return true;
  }
  return false;
}

// Türkçe anahtar kelimeler
const char* KW_SELAM_TR[] = {"merhaba", "selam", "hey", "naber", "nasilsin", "nasıl", "günaydın", "iyi akşam", "iyi gün"};
const char* KW_TARIH_TR[] = {"tarih", "tarihi", "tarihçe", "ne zaman", "hangi yüzyıl", "roma", "bizans", "antik", "eski", "geçmiş", "kuruluş"};
const char* KW_MITHRAS_TR[] = {"mithras", "mitra", "tapınak", "tapinak", "din", "inanç", "kült", "boğa", "boga", "tören", "gizem", "yeraltı"};
const char* KW_MIMARI_TR[] = {"mimari", "yapı", "yapi", "bina", "sur", "kule", "duvar", "mezar", "tünel", "tunel", "oda", "inşa", "inşaat"};
const char* KW_SU_TR[] = {"su", "sarnıç", "sarnic", "hamam", "banyo", "kanal", "çeşme", "cesme", "yağmur", "mühendis"};
const char* KW_ASKERI_TR[] = {"asker", "ordu", "savaş", "savas", "silah", "zırh", "zirh", "kılıç", "kilic", "eğitim", "disiplin", "legion"};
const char* KW_SASANI_TR[] = {"sasani", "pers", "iran", "düşman", "dusman", "saldırı", "kuşatma", "kusatma"};
const char* KW_YASAM_TR[] = {"yaşam", "yasam", "günlük", "gündelik", "yemek", "eğlence", "kadın", "kadin", "aile", "mektup"};
const char* KW_IPEK_TR[] = {"ipek", "yol", "ticaret", "kervan", "deve", "tüccar", "tuccar", "çin", "cin", "baharat"};
const char* KW_ARKEOLOJI_TR[] = {"arkeoloji", "kazı", "kazi", "buluntu", "keşif", "kesif", "üniversite", "bilim"};
const char* KW_UNESCO_TR[] = {"unesco", "miras", "koruma", "dünya", "dunya", "liste", "evrensel"};
const char* KW_ZIYARET_TR[] = {"ziyaret", "giriş", "giris", "bilet", "saat", "konum", "nerede", "nasıl giderim", "adres", "ulaşım", "ulasim"};
const char* KW_EFSANE_TR[] = {"efsane", "gizem", "hazine", "define", "hayalet", "tünel", "sır", "sir", "bilinmeyen"};
const char* KW_VEDA_TR[] = {"görüşürüz", "gorusuruz", "hoşça", "hosca", "güle güle", "bye", "çıkış", "cikis", "teşekkür", "tesekkur", "sağol", "sagol"};
const char* KW_EVET_TR[] = {"evet", "tabii", "tabi", "olur", "anlat", "devam", "isterim", "istiyorum", "söyle", "soyle", "lütfen", "lutfen"};
const char* KW_HAYIR_TR[] = {"hayır", "hayir", "yok", "gerek yok", "istemiyorum", "başka", "baska", "değiştir", "degistir"};

// İngilizce anahtar kelimeler
const char* KW_SELAM_EN[] = {"hello", "hi", "hey", "greetings", "good morning", "good evening", "howdy"};
const char* KW_TARIH_EN[] = {"history", "historical", "when", "century", "roman", "byzantine", "ancient", "old", "past", "founded"};
const char* KW_MITHRAS_EN[] = {"mithras", "mithra", "temple", "religion", "belief", "cult", "bull", "ceremony", "mystery", "underground"};
const char* KW_MIMARI_EN[] = {"architecture", "building", "wall", "tower", "tomb", "tunnel", "room", "construction", "structure"};
const char* KW_SU_EN[] = {"water", "cistern", "bath", "channel", "fountain", "rain", "engineer", "aqueduct"};
const char* KW_ASKERI_EN[] = {"soldier", "army", "military", "war", "weapon", "armor", "sword", "training", "discipline", "legion"};
const char* KW_ZIYARET_EN[] = {"visit", "entrance", "ticket", "hours", "location", "where", "how to get", "address", "transport"};
const char* KW_VEDA_EN[] = {"goodbye", "bye", "see you", "farewell", "thanks", "thank you", "exit"};

// ══════════════════════════════════════════════════════════════════════════════
// CHATBOT MOTORU
// ══════════════════════════════════════════════════════════════════════════════

String getResponse(String query) {
  query.toLowerCase();
  query.trim();
  
  // Dil algılama ve değiştirme
  if (query.indexOf("english") != -1 || query.indexOf("ingilizce") != -1) {
    currentLang = EN;
    return "🌍 Switched to English! I'm Zerzevan Castle's AI guide. How can I help you?";
  }
  if (query.indexOf("türkçe") != -1 || query.indexOf("turkce") != -1 || query.indexOf("turkish") != -1) {
    currentLang = TR;
    return "🌍 Türkçe'ye geçildi! Zerzevan Kalesi AI rehberiyim. Size nasıl yardımcı olabilirim?";
  }
  
  // Rastgele cevap seçici
  randomSeed(millis());
  
  // TÜRKÇE CEVAPLAR
  if (currentLang == TR) {
    // Selamlaşma
    if (containsAny(query, KW_SELAM_TR, 9)) {
      lastContext = "selam";
      return String(TR_SELAM[random(4)]);
    }
    
    // Vedalaşma
    if (containsAny(query, KW_VEDA_TR, 11)) {
      lastContext = "";
      return String(TR_VEDA[random(4)]);
    }
    
    // Evet/Devam
    if (containsAny(query, KW_EVET_TR, 10) && lastContext != "") {
      if (lastContext == "tarih") return String(TR_TARIH[random(1, 4)]);
      if (lastContext == "mithras") return String(TR_MITHRAS[random(1, 5)]);
      if (lastContext == "mimari") return String(TR_MIMARI[random(1, 4)]);
      if (lastContext == "su") return String(TR_SU[random(1, 4)]);
      if (lastContext == "askeri") return String(TR_ASKERI[random(1, 5)]);
    }
    
    // Tarih
    if (containsAny(query, KW_TARIH_TR, 11)) {
      lastContext = "tarih";
      return String(TR_TARIH[random(4)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Mithras
    if (containsAny(query, KW_MITHRAS_TR, 12)) {
      lastContext = "mithras";
      return String(TR_MITHRAS[random(5)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Mimari
    if (containsAny(query, KW_MIMARI_TR, 12)) {
      lastContext = "mimari";
      return String(TR_MIMARI[random(4)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Su Sistemleri
    if (containsAny(query, KW_SU_TR, 10)) {
      lastContext = "su";
      return String(TR_SU[random(4)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Askeri Yaşam
    if (containsAny(query, KW_ASKERI_TR, 11)) {
      lastContext = "askeri";
      return String(TR_ASKERI[random(5)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Sasani
    if (containsAny(query, KW_SASANI_TR, 8)) {
      lastContext = "sasani";
      return String(TR_SASANI[random(4)]);
    }
    
    // Günlük Yaşam
    if (containsAny(query, KW_YASAM_TR, 10)) {
      lastContext = "yasam";
      return String(TR_YASAM[random(4)]);
    }
    
    // İpek Yolu
    if (containsAny(query, KW_IPEK_TR, 10)) {
      lastContext = "ipek";
      return String(TR_IPEK[random(4)]);
    }
    
    // Arkeoloji
    if (containsAny(query, KW_ARKEOLOJI_TR, 7)) {
      lastContext = "arkeoloji";
      return String(TR_ARKEOLOJI[random(4)]);
    }
    
    // UNESCO
    if (containsAny(query, KW_UNESCO_TR, 7)) {
      lastContext = "unesco";
      return String(TR_UNESCO[random(4)]);
    }
    
    // Ziyaret
    if (containsAny(query, KW_ZIYARET_TR, 11)) {
      lastContext = "ziyaret";
      return String(TR_ZIYARET[random(4)]);
    }
    
    // Efsaneler
    if (containsAny(query, KW_EFSANE_TR, 8)) {
      lastContext = "efsane";
      return String(TR_EFSANE[random(4)]);
    }
    
    // Zerzevan genel
    if (query.indexOf("zerzevan") != -1 || query.indexOf("kale") != -1) {
      lastContext = "genel";
      return String(TR_GENEL[random(4)]) + "\n\n" + String(TR_DEVAM[random(4)]);
    }
    
    // Bilinmeyen
    return String(TR_BILINMEYEN[random(4)]);
  }
  
  // İNGİLİZCE CEVAPLAR
  else {
    // Greetings
    if (containsAny(query, KW_SELAM_EN, 7)) {
      lastContext = "greeting";
      return String(EN_SELAM[random(4)]);
    }
    
    // Farewell
    if (containsAny(query, KW_VEDA_EN, 7)) {
      lastContext = "";
      return String(EN_VEDA[random(4)]);
    }
    
    // History
    if (containsAny(query, KW_TARIH_EN, 10)) {
      lastContext = "history";
      return String(EN_TARIH[random(4)]);
    }
    
    // Mithras
    if (containsAny(query, KW_MITHRAS_EN, 10)) {
      lastContext = "mithras";
      return String(EN_MITHRAS[random(5)]);
    }
    
    // Architecture
    if (containsAny(query, KW_MIMARI_EN, 9)) {
      lastContext = "architecture";
      return String(EN_MIMARI[random(4)]);
    }
    
    // Water
    if (containsAny(query, KW_SU_EN, 8)) {
      lastContext = "water";
      return String(EN_SU[random(4)]);
    }
    
    // Military
    if (containsAny(query, KW_ASKERI_EN, 10)) {
      lastContext = "military";
      return String(EN_ASKERI[random(5)]);
    }
    
    // Visit
    if (containsAny(query, KW_ZIYARET_EN, 9)) {
      lastContext = "visit";
      return String(EN_ZIYARET[random(4)]);
    }
    
    // Zerzevan general
    if (query.indexOf("zerzevan") != -1 || query.indexOf("castle") != -1) {
      lastContext = "general";
      return String(EN_GENEL[random(4)]);
    }
    
    // Unknown
    return String(EN_BILINMEYEN[random(4)]);
  }
}

// ══════════════════════════════════════════════════════════════════════════════
// HTML ARAYÜZ
// ══════════════════════════════════════════════════════════════════════════════

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Zerzevan Kalesi AI Rehber</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    :root {
      --primary: #8B4513;
      --primary-dark: #5D2E0C;
      --secondary: #DAA520;
      --bg-dark: #1a1a2e;
      --bg-card: #16213e;
      --text: #eaeaea;
      --text-muted: #a0a0a0;
      --accent: #e94560;
      --success: #4ecca3;
    }
    
    body {
      font-family: 'Segoe UI', system-ui, sans-serif;
      background: linear-gradient(135deg, var(--bg-dark) 0%, #0f0f23 100%);
      min-height: 100vh;
      color: var(--text);
    }
    
    .container {
      max-width: 600px;
      margin: 0 auto;
      padding: 10px;
      height: 100vh;
      display: flex;
      flex-direction: column;
    }
    
    /* Header */
    .header {
      text-align: center;
      padding: 15px;
      background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
      border-radius: 20px;
      margin-bottom: 10px;
      box-shadow: 0 10px 30px rgba(139, 69, 19, 0.3);
    }
    
    .header h1 {
      font-size: 1.5em;
      margin-bottom: 5px;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
    }
    
    .header p {
      font-size: 0.85em;
      opacity: 0.9;
    }
    
    /* Language Selector */
    .lang-selector {
      display: flex;
      justify-content: center;
      gap: 10px;
      margin: 10px 0;
    }
    
    .lang-btn {
      padding: 8px 20px;
      border: 2px solid var(--secondary);
      background: transparent;
      color: var(--secondary);
      border-radius: 20px;
      cursor: pointer;
      font-size: 0.9em;
      transition: all 0.3s;
    }
    
    .lang-btn.active, .lang-btn:hover {
      background: var(--secondary);
      color: var(--bg-dark);
    }
    
    /* Chat Area */
    .chat-area {
      flex: 1;
      overflow-y: auto;
      padding: 10px;
      background: var(--bg-card);
      border-radius: 20px;
      margin-bottom: 10px;
      scroll-behavior: smooth;
    }
    
    .message {
      margin-bottom: 15px;
      animation: fadeIn 0.3s ease;
    }
    
    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(10px); }
      to { opacity: 1; transform: translateY(0); }
    }
    
    .message.user {
      text-align: right;
    }
    
    .message.bot {
      text-align: left;
    }
    
    .message-content {
      display: inline-block;
      padding: 12px 18px;
      border-radius: 18px;
      max-width: 85%;
      line-height: 1.5;
      white-space: pre-wrap;
    }
    
    .message.user .message-content {
      background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
      border-bottom-right-radius: 5px;
    }
    
    .message.bot .message-content {
      background: linear-gradient(135deg, #2d3a5f 0%, #1e2747 100%);
      border-bottom-left-radius: 5px;
    }
    
    /* Quick Topics */
    .quick-topics {
      display: flex;
      flex-wrap: wrap;
      gap: 8px;
      justify-content: center;
      margin-bottom: 10px;
    }
    
    .topic-btn {
      padding: 8px 15px;
      background: var(--bg-card);
      border: 1px solid var(--secondary);
      color: var(--secondary);
      border-radius: 20px;
      font-size: 0.8em;
      cursor: pointer;
      transition: all 0.3s;
    }
    
    .topic-btn:hover {
      background: var(--secondary);
      color: var(--bg-dark);
      transform: scale(1.05);
    }
    
    /* Input Area */
    .input-area {
      display: flex;
      gap: 10px;
      padding: 10px;
      background: var(--bg-card);
      border-radius: 25px;
    }
    
    .input-area input {
      flex: 1;
      padding: 12px 20px;
      border: none;
      border-radius: 20px;
      background: var(--bg-dark);
      color: var(--text);
      font-size: 1em;
      outline: none;
    }
    
    .input-area input::placeholder {
      color: var(--text-muted);
    }
    
    .btn-icon {
      width: 48px;
      height: 48px;
      border-radius: 50%;
      border: none;
      cursor: pointer;
      font-size: 1.3em;
      display: flex;
      align-items: center;
      justify-content: center;
      transition: all 0.3s;
    }
    
    .btn-send {
      background: linear-gradient(135deg, var(--primary) 0%, var(--primary-dark) 100%);
      color: white;
    }
    
    .btn-mic {
      background: linear-gradient(135deg, var(--accent) 0%, #c73e54 100%);
      color: white;
    }
    
    .btn-mic.recording {
      animation: pulse 1s infinite;
      background: #ff0000;
    }
    
    @keyframes pulse {
      0%, 100% { transform: scale(1); }
      50% { transform: scale(1.1); }
    }
    
    .btn-speak {
      background: linear-gradient(135deg, var(--success) 0%, #3ba385 100%);
      color: white;
    }
    
    .btn-icon:hover {
      transform: scale(1.1);
    }
    
    /* Typing indicator */
    .typing {
      display: flex;
      gap: 5px;
      padding: 15px;
    }
    
    .typing span {
      width: 8px;
      height: 8px;
      background: var(--secondary);
      border-radius: 50%;
      animation: bounce 1.4s infinite;
    }
    
    .typing span:nth-child(2) { animation-delay: 0.2s; }
    .typing span:nth-child(3) { animation-delay: 0.4s; }
    
    @keyframes bounce {
      0%, 80%, 100% { transform: translateY(0); }
      40% { transform: translateY(-10px); }
    }
    
    /* Footer */
    .footer {
      text-align: center;
      padding: 10px;
      font-size: 0.75em;
      color: var(--text-muted);
    }
    
    /* Scrollbar */
    ::-webkit-scrollbar {
      width: 6px;
    }
    
    ::-webkit-scrollbar-track {
      background: var(--bg-dark);
    }
    
    ::-webkit-scrollbar-thumb {
      background: var(--primary);
      border-radius: 3px;
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Header -->
    <div class="header">
      <h1>🏰 Zerzevan Kalesi</h1>
      <p>Yapay Zeka Rehberiniz | AI Guide</p>
    </div>
    
    <!-- Language Selector -->
    <div class="lang-selector">
      <button class="lang-btn active" onclick="setLang('tr')">🇹🇷 Türkçe</button>
      <button class="lang-btn" onclick="setLang('en')">🇬🇧 English</button>
    </div>
    
    <!-- Chat Area -->
    <div class="chat-area" id="chatArea">
      <div class="message bot">
        <div class="message-content">
          Merhaba! 👋 Zerzevan Kalesi AI Rehberine hoş geldiniz! 
          
Size bu antik Roma kalesini tanıtmaktan mutluluk duyarım. 

🎤 Sesli soru sormak için mikrofon butonunu kullanın
⌨️ Veya yazarak soru sorun

Ne öğrenmek istersiniz?
        </div>
      </div>
    </div>
    
    <!-- Quick Topics -->
    <div class="quick-topics" id="quickTopics">
      <button class="topic-btn" onclick="askTopic('Tarih')">📜 Tarih</button>
      <button class="topic-btn" onclick="askTopic('Mithras')">🔥 Mithras</button>
      <button class="topic-btn" onclick="askTopic('Mimari')">🏗️ Mimari</button>
      <button class="topic-btn" onclick="askTopic('Ziyaret')">📍 Ziyaret</button>
      <button class="topic-btn" onclick="askTopic('Askerler')">⚔️ Askerler</button>
    </div>
    
    <!-- Input Area -->
    <div class="input-area">
      <button class="btn-icon btn-mic" id="micBtn" onclick="toggleVoice()">🎤</button>
      <input type="text" id="userInput" placeholder="Sorunuzu yazın..." onkeypress="handleKey(event)">
      <button class="btn-icon btn-speak" id="speakBtn" onclick="speakLast()">🔊</button>
      <button class="btn-icon btn-send" onclick="sendMessage()">➤</button>
    </div>
    
    <!-- Footer -->
    <div class="footer">
      Zerzevan Kalesi Arkeoloji Projesi © 2024
    </div>
  </div>
  
  <script>
    let currentLang = 'tr';
    let isRecording = false;
    let recognition = null;
    let lastBotMessage = '';
    
    // Web Speech API desteği
    const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
    const synthesis = window.speechSynthesis;
    
    if (SpeechRecognition) {
      recognition = new SpeechRecognition();
      recognition.continuous = false;
      recognition.interimResults = false;
      
      recognition.onresult = (event) => {
        const text = event.results[0][0].transcript;
        document.getElementById('userInput').value = text;
        sendMessage();
      };
      
      recognition.onend = () => {
        isRecording = false;
        document.getElementById('micBtn').classList.remove('recording');
      };
      
      recognition.onerror = () => {
        isRecording = false;
        document.getElementById('micBtn').classList.remove('recording');
      };
    }
    
    function setLang(lang) {
      currentLang = lang;
      document.querySelectorAll('.lang-btn').forEach(btn => btn.classList.remove('active'));
      event.target.classList.add('active');
      
      if (recognition) {
        recognition.lang = lang === 'tr' ? 'tr-TR' : 'en-US';
      }
      
      const topics = document.getElementById('quickTopics');
      if (lang === 'tr') {
        topics.innerHTML = `
          <button class="topic-btn" onclick="askTopic('Tarih')">📜 Tarih</button>
          <button class="topic-btn" onclick="askTopic('Mithras')">🔥 Mithras</button>
          <button class="topic-btn" onclick="askTopic('Mimari')">🏗️ Mimari</button>
          <button class="topic-btn" onclick="askTopic('Ziyaret')">📍 Ziyaret</button>
          <button class="topic-btn" onclick="askTopic('Askerler')">⚔️ Askerler</button>
        `;
        document.getElementById('userInput').placeholder = 'Sorunuzu yazın...';
      } else {
        topics.innerHTML = `
          <button class="topic-btn" onclick="askTopic('History')">📜 History</button>
          <button class="topic-btn" onclick="askTopic('Mithras')">🔥 Mithras</button>
          <button class="topic-btn" onclick="askTopic('Architecture')">🏗️ Architecture</button>
          <button class="topic-btn" onclick="askTopic('Visit')">📍 Visit</button>
          <button class="topic-btn" onclick="askTopic('Soldiers')">⚔️ Soldiers</button>
        `;
        document.getElementById('userInput').placeholder = 'Type your question...';
      }
      
      // Dil değişikliği mesajı gönder
      sendQuery(lang === 'tr' ? 'Türkçe konuş' : 'English please');
    }
    
    function toggleVoice() {
      if (!recognition) {
        alert(currentLang === 'tr' ? 'Tarayıcınız ses tanımayı desteklemiyor!' : 'Your browser does not support voice recognition!');
        return;
      }
      
      if (isRecording) {
        recognition.stop();
        isRecording = false;
        document.getElementById('micBtn').classList.remove('recording');
      } else {
        recognition.lang = currentLang === 'tr' ? 'tr-TR' : 'en-US';
        recognition.start();
        isRecording = true;
        document.getElementById('micBtn').classList.add('recording');
      }
    }
    
    function speakLast() {
      if (!synthesis) {
        alert(currentLang === 'tr' ? 'Tarayıcınız ses sentezini desteklemiyor!' : 'Your browser does not support speech synthesis!');
        return;
      }
      
      if (lastBotMessage) {
        synthesis.cancel();
        const utterance = new SpeechSynthesisUtterance(lastBotMessage.replace(/[🏰📜🔥🏗️💧⚔️🛡️🍷🐪🔍🌟📍👋👻🤔💬🎭⭐💪🏊🛁🏃⚖️💎🖌️🛸🏆🎟️💰🤫👀☀️📞🎯😅💑]/g, ''));
        utterance.lang = currentLang === 'tr' ? 'tr-TR' : 'en-US';
        synthesis.speak(utterance);
      }
    }
    
    function askTopic(topic) {
      document.getElementById('userInput').value = topic;
      sendMessage();
    }
    
    function handleKey(event) {
      if (event.key === 'Enter') {
        sendMessage();
      }
    }
    
    function addMessage(text, isUser) {
      const chatArea = document.getElementById('chatArea');
      const msgDiv = document.createElement('div');
      msgDiv.className = `message ${isUser ? 'user' : 'bot'}`;
      msgDiv.innerHTML = `<div class="message-content">${text}</div>`;
      chatArea.appendChild(msgDiv);
      chatArea.scrollTop = chatArea.scrollHeight;
      
      if (!isUser) {
        lastBotMessage = text;
      }
    }
    
    function showTyping() {
      const chatArea = document.getElementById('chatArea');
      const typingDiv = document.createElement('div');
      typingDiv.id = 'typingIndicator';
      typingDiv.className = 'message bot';
      typingDiv.innerHTML = '<div class="typing"><span></span><span></span><span></span></div>';
      chatArea.appendChild(typingDiv);
      chatArea.scrollTop = chatArea.scrollHeight;
    }
    
    function hideTyping() {
      const typing = document.getElementById('typingIndicator');
      if (typing) typing.remove();
    }
    
    function sendMessage() {
      const input = document.getElementById('userInput');
      const text = input.value.trim();
      if (!text) return;
      
      addMessage(text, true);
      input.value = '';
      sendQuery(text);
    }
    
    function sendQuery(query) {
      showTyping();
      
      fetch('/ask?q=' + encodeURIComponent(query))
        .then(response => response.text())
        .then(data => {
          hideTyping();
          addMessage(data, false);
        })
        .catch(error => {
          hideTyping();
          addMessage(currentLang === 'tr' ? 'Bağlantı hatası! Tekrar deneyin.' : 'Connection error! Please try again.', false);
        });
    }
  </script>
</body>
</html>
)rawliteral";

// ══════════════════════════════════════════════════════════════════════════════
// WEB SUNUCU HANDLERLARI
// ══════════════════════════════════════════════════════════════════════════════

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleAsk() {
  String query = server.arg("q");
  String response = getResponse(query);
  server.send(200, "text/plain; charset=utf-8", response);
}

void handleNotFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// ══════════════════════════════════════════════════════════════════════════════
// ANA PROGRAM
// ══════════════════════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("═══════════════════════════════════════════════════════");
  Serial.println("   ZERZEVAN KALESİ - GELİŞMİŞ AI REHBER v2.0");
  Serial.println("═══════════════════════════════════════════════════════");
  
  // WiFi Access Point başlat
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID, AP_PASS);
  
  Serial.print("📡 WiFi Ağı: ");
  Serial.println(AP_SSID);
  Serial.print("🌐 IP Adresi: ");
  Serial.println(WiFi.softAPIP());
  
  // DNS sunucusu (Captive Portal için)
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("🔗 Captive Portal aktif");
  
  // Web sunucu rotaları
  server.on("/", handleRoot);
  server.on("/ask", handleAsk);
  server.on("/generate_204", handleRoot);  // Android
  server.on("/fwlink", handleRoot);         // Windows
  server.on("/hotspot-detect.html", handleRoot); // iOS
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("✅ Sunucu başlatıldı!");
  Serial.println("═══════════════════════════════════════════════════════");
  Serial.println("📱 Telefonunuzdan 'Zerzevan Kalesi Rehber' WiFi'ye bağlanın");
  Serial.println("═══════════════════════════════════════════════════════");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
