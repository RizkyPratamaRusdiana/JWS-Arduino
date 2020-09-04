#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#include  "RTClib.h"
#include  <DHT.h>;
#include  "PrayerTimes.h"
 

#define   DHTPIN 2    
#define   DHTTYPE DHT22   

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);

char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
char namaBulan [12][12] = {"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agt", "Sep", "Oct", "Nov", "Des"};

int screenWidth = 16;
int screenHeight = 2;
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

int hum, temp, hourupg, minupg, yearupg, monthupg, dayupg;
int menu = 0;

double waktu[sizeof(TimeName)/sizeof(char*)];
  
static const char* NamaWaktu[] ={
      "Imsak",
      "Subuh",
      "Syuruq",
      "Dzuhur",
      "Ashar",
      "Terbenam",
      "Maghrib",
      "Isya'",
      "Count"
};

const byte JUMLAH_WAKTU_DEFAULT = sizeof(waktu)/sizeof(double);
const byte JUMLAH_WAKTU = sizeof(NamaWaktu)/sizeof(char*);
const byte DATA_WAKTU = 2; // jam dan menit

const int UP_BUTTONon = 3;     
const int DOWN_BUTTONon = 4;

byte WaktuPenting[JUMLAH_WAKTU][DATA_WAKTU];

byte temperatur[8] = 
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};


boolean lastDownONButton = LOW;
boolean currentDownONButton = LOW;
boolean lastUpONButton = LOW;
boolean currentUpONButton = LOW;
  
  
// pengaturan bujur lintang (Sindangkerta)
float _lat = -6.985551;    // lintang
float _lng = 107.413885;   // bujur
  
// timezone
int _timezone = 7; // zona waktu WIB=7, WITA=8, WIT=9
  
// sudut subuh dan maghrib Indonesia
int _sudut_subuh = 20;
int _sudut_isyak = 18; 


String DuaDigit(byte angka){
    if( angka > 9 ){
        return String(angka);
    } else {
        String tmp = "0";
        tmp.concat(angka);
        return tmp;
    }
}



//=====================> RESET ARDUINO <=====================\\
void(* ku_reset) (void) = 0;



void setup() {
  Serial.begin(19200);
  lcd.begin(screenWidth,screenHeight);
  dht.begin(); 
  set_calc_method(ISNA);  // metode perhitungan
  set_asr_method(Shafii); // madzhab?
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(_sudut_subuh);     // sudut waktu subuh
  set_isha_angle(_sudut_isyak);     // sudut waktu isyak

  lcd.createChar(1, temperatur);
  
  if (! rtc.begin()) {
    Serial.println("rtc TIDAK TERBACA");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("rtc is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//update rtc dari waktu komputer
  }

  opening();
}




//*********************************************************//
boolean debounce(boolean last, int pin)
  {
  boolean current = digitalRead(pin);
  if (last != current)
  {
  delay(5);
  current = digitalRead(pin);
  } 
  return current;
  }
  
//*********************************************************//




void opening(){
  lcd.begin(16, 2);
  lcd.setCursor(5,0);
  lcd.print("N");delay(300); lcd.print("G");delay(300);
  lcd.print("U");delay(300); lcd.print("L");delay(300);
  lcd.print("I");delay(300); lcd.print("K");delay(300);

  lcd.setCursor(0,1);
  lcd.print("N");delay(200); lcd.print("O");delay(200);
  lcd.print("N");delay(200); lcd.print("O");delay(200);
  lcd.print("M");delay(200); lcd.print("A");delay(200);
  lcd.print("N  ");delay(200); lcd.print("M");delay(200);
  lcd.print("O");delay(200); lcd.print("T");delay(200);
  lcd.print("E");delay(200); lcd.print("K");delay(200);
  lcd.print("A");delay(200); lcd.print("R");delay(2000);
  lcd.clear();
}




void loop() {
  
  lcd.setCursor(0,0);
  lcd.write(1);
  tampilanJam();
  runningText();
  lcd.clear();
  
  currentDownONButton = debounce(lastDownONButton, DOWN_BUTTONon);
  currentUpONButton = debounce(lastUpONButton, UP_BUTTONon);
  

//  if (lastUpONButton== LOW && currentUpONButton == HIGH)
//  {
//   menu=menu+1;
//  }
//  if (menu==0)
//    {
//     lcd.setCursor(0,0);
//     lcd.write(1);
//     tampilanJam();
//     runningText();
//     lcd.clear();
//    }
//  if (menu==1)
//    {
//    DisplaySetHour();
//    runningText();
//    }
//  if (menu==2)
//    {
//    DisplaySetMinute();
//    runningText();
//    }
//  if (menu==3)
//    {
//    DisplaySetDay();
//    runningText();
//    }
//  if (menu==4)
//    {
//    DisplaySetMonth();
//    runningText();
//    }
//  if (menu==5)
//    {
//    DisplaySetYear();
//    runningText();
//    }
//  if (menu==6)
//    {
//    SavingDateTime();
//    runningText();
//    delay(2000);
//    menu=0;
//    }
}



void DisplaySetHour()
{
  lcd.clear();
  DateTime now = rtc.now();
  if(lastDownONButton== LOW && currentDownONButton == HIGH)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("ATUR JAM = ");
  lcd.setCursor(14,0);
  if(hourupg < 10)
  {
    lcd.print("0");
  }
  lcd.print(hourupg,DEC);
  delay(200);
}

//*********************************************************//

void DisplaySetMinute()
{
  lcd.clear();
  DateTime now = rtc.now();
  if(lastDownONButton== LOW && currentDownONButton == HIGH)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("ATUR MENIT = ");
  lcd.setCursor(14,0);
  if(minupg < 10)
  {
    lcd.print("0");
  }
  lcd.print(minupg,DEC);
  delay(200);
}

//*********************************************************//

void DisplaySetYear()
{
  lcd.clear();
  DateTime now = rtc.now();
  if(lastDownONButton== LOW && currentDownONButton == HIGH)
  {    
    yearupg=yearupg+1;
  }
  lcd.setCursor(0,0);
  lcd.print("ATUR TAHUN = ");
  lcd.setCursor(14,0);
  if(yearupg < 10)
  {
    lcd.print("0");
  }
  lcd.print(yearupg,DEC);
  delay(200);
}

//*********************************************************//

void DisplaySetMonth()
{
  lcd.clear();
  DateTime now = rtc.now();
  if(lastDownONButton== LOW && currentDownONButton == HIGH)
  {
    if (monthupg==12)
    {
      monthupg=1;
    }
    else
    {
      monthupg=monthupg+1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("ATUR BULAN = ");
  lcd.setCursor(14, 0);
  if(monthupg < 10)
  {
    lcd.print("0");
  }
  lcd.print(monthupg,DEC);
  delay(200);
}

//*********************************************************//

void DisplaySetDay()
{
  lcd.clear();
  DateTime now = rtc.now();
  if(lastDownONButton== LOW && currentDownONButton == HIGH)
  {
    if (dayupg==31)
    {
      dayupg=1;
    }
    else
    {
      dayupg=dayupg+1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("ATUR TGL = ");
  lcd.setCursor(14,0);
  if(dayupg < 10)
  {
    lcd.print("0");
  }
  lcd.print(dayupg,DEC);
  delay(200);
}

//*********************************************************//

void SavingDateTime()
{
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("MENYIMPAN!");
  rtc.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
  delay(200);
}




//=====================> TAMPILAN JAM <=====================\\

void tampilanJam(){
  DateTime now = rtc.now();
  
   
  int jam = now.hour() ;
  int menit = now.minute();
  int detik = now.second();

  
//=====================> JAM/MENIT/DETIK <=====================\\

//  lcd.clear();
  lcd.setCursor(8, 0);
  if (jam<=9)
  {
    lcd.print("0");
  }
  lcd.print(jam);

  lcd.print(':');
  if (menit<=9)
  {
    lcd.print("0");
  }
  lcd.print(menit);

  lcd.print(':');
  if (detik<=9)
  {
    lcd.print("0");
  }
  lcd.print(detik);

//  batterylevel(15,0);

//=====================> SENSOR SUHU <=====================\\
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.print(temp);
  lcd.print((char) 223);
  lcd.print("C");
  hum = dht.readHumidity();
  temp= dht.readTemperature();

}




void runningText(){
  
  DateTime now = rtc.now();
  

//=====================> TANGGAL/BULAN/TAHUN <=====================\\
    
  int tanggal = now.day();
//  int bulan = now.month();
  int tahun = now.year();
  
  String hari = namaHari[now.dayOfTheWeek()];
  String bulan = namaBulan[now.month()-1];
  String tgl, bln, thn; 
  String pemisah = "/";
  String pemisah2 = ", ";
  
  tgl = String(tanggal);
//  bln = String(bulan);
  thn = String(tahun);

  String printTanggal = hari + pemisah2 + tgl + pemisah + bulan + pemisah + tahun ;




//=====================> WAKTU SHALAT <=====================\\

  String spasi = " ";
  String spasi2 = "  ";
  String titikDua = ":";
  int _tahun = now.year();
  int _bulan = now.month();
  int _hari = now.day(); // tanggal
   
  Serial.println("  ===========================");
  Serial.print("   Jdwl Sholat Tgl. ");
  Serial.print(_hari);
  Serial.print("/");
  Serial.print(_bulan);
  Serial.print("/");
  Serial.print(_tahun);
  Serial.println();
  Serial.println("  ===========================");
     
  for(byte i=0; i<JUMLAH_WAKTU-1; i++){
         
      if( i == 5 ) continue;
      Serial.print("\t");
      Serial.print( NamaWaktu[i] );
      Serial.print("\t");
      Serial.print( DuaDigit(WaktuPenting[i][0]) );
      Serial.print(":");
      Serial.println( DuaDigit(WaktuPenting[i][1]) );
         
  }
     
  Serial.println();

  byte WaktuShalat[JUMLAH_WAKTU][DATA_WAKTU];
  get_prayer_times(_tahun, _bulan, _hari, _lat, _lng, _timezone, waktu);
    byte waktu_custom = 0;
    for(byte i=0; i<JUMLAH_WAKTU_DEFAULT-1; i++){
        int jam, menit;
        get_float_time_parts(waktu[i], jam, menit);
        if( i == 0 ){
            WaktuPenting[waktu_custom][0] = jam;
            WaktuPenting[waktu_custom][1] = menit-10;
            waktu_custom++;
            WaktuPenting[waktu_custom][0] = jam;
            WaktuPenting[waktu_custom][1] = menit;
        } else {
            WaktuPenting[waktu_custom][0] = jam;
            WaktuPenting[waktu_custom][1] = menit;
        }
        waktu_custom++;
    }
  String imsyak = spasi2 + NamaWaktu[0] + spasi + DuaDigit(WaktuPenting[0][0]) + titikDua + DuaDigit(WaktuPenting[0][1]);
  String solatSubuh = spasi2 + NamaWaktu[1] + spasi + DuaDigit(WaktuPenting[1][0]) + titikDua + DuaDigit(WaktuPenting[1][1]);
  String solatDzuhur = spasi2 + NamaWaktu[3] + spasi + DuaDigit(WaktuPenting[3][0]) + titikDua + DuaDigit(WaktuPenting[3][1]);
  String solatAshar = spasi2 + NamaWaktu[4] + spasi + DuaDigit(WaktuPenting[4][0]) + titikDua + DuaDigit(WaktuPenting[4][1]);
  String solatMaghrib = spasi2 + NamaWaktu[6] + spasi + DuaDigit(WaktuPenting[6][0]) + titikDua + DuaDigit(WaktuPenting[6][1]);
  String solatIsya = spasi2 + NamaWaktu[7] + spasi + DuaDigit(WaktuPenting[7][0]) + titikDua + DuaDigit(WaktuPenting[7][1]);

  String printRunningSholat = solatSubuh + solatDzuhur + solatAshar + solatMaghrib + solatIsya; 




//=====================> PRINT RUNNING TEXT <=====================\\

  String printRunning = printTanggal + printRunningSholat ;
  
  lcd.setCursor(scrollCursor, 1);
  lcd.print(printRunning.substring(stringStart,stringStop));
  
  delay(300);
//  lcd.clear();
  if(stringStart == 0 && scrollCursor > 0){
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop){
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == printRunning.length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }
}



//=====================> PRINT BATTERY LEVEL <=====================\\

void batterylevel(int xpos,int ypos){

// Membaca tegangan dan mengkonversi ke volt
  double curvolt = double( readVcc() ) / 1000;

// Membaca jika tegangan lebih besar dari 4.2 Volt, maka perangkat sedang di cas
  if(curvolt > 4.2)
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B10101,
    B10001,
    B11011,
    B11011,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 4.2 && curvolt > 4.0)
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 4.0 && curvolt > 3.8)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 3.8 && curvolt > 3.6)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 3.6 && curvolt > 3.4)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 3.4 && curvolt > 3.2)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 3.2 && curvolt > 3.0)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt < 3.0)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
}

// Membaca tegangan internal
  long readVcc() {
  long result;
  
// Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}
