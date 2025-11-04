#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.hpp"

static const char *CFG_PATH  = "/config.json";
static const char *HITS_PATH = "/hits.json";

static void setDefaults(BadgeConfig &c){
  c.name         = "Unconfigured";
  c.id           = 0;
  c.colorRGB     = 0x00FF00;
  c.brightness   = 7;
  c.unlockedMask = 0;
  c.score        = 0;
  c.sentCount    = 0;
  c.userCharmId  = 0xff;
  c.sleepDisabled = false;            // <-- NEW default
}

bool loadConfig(BadgeConfig &out){
  if (!LittleFS.exists(CFG_PATH)) {
    setDefaults(out);
    saveConfig(out);
    return true;
  }
  File f = LittleFS.open(CFG_PATH, "r");
  if (!f) { setDefaults(out); return false; }

  JsonDocument doc;
  DeserializationError e = deserializeJson(doc, f);
  f.close();
  if (e) { setDefaults(out); return false; }

  const char* nm = doc["name"] | "Badge";
  out.name         = String(nm);
  out.id           = uint16_t(doc["id"]           | 0);
  out.colorRGB     = uint32_t(doc["colorRGB"]     | 0x00FF00);
  out.brightness   = uint8_t(doc["brightness"]    | 15);
  out.unlockedMask = uint32_t(doc["unlockedMask"] | 0);
  out.score        = uint32_t(doc["score"]        | 0);
  out.sentCount    = uint32_t(doc["sentCount"]    | 0);
  out.userCharmId  = uint32_t(doc["userCharmId"]  | 0xFF);
  out.sleepDisabled= bool(doc["sleepDisabled"]    | false);   // <-- NEW

  return true;
}

bool saveConfig(const BadgeConfig &in){
  JsonDocument doc;
  doc["name"]         = in.name;
  doc["id"]           = in.id;
  doc["colorRGB"]     = in.colorRGB;
  doc["brightness"]   = in.brightness;
  doc["unlockedMask"] = in.unlockedMask;
  doc["score"]        = in.score;
  doc["sentCount"]    = in.sentCount;
  doc["userCharmId"]  = in.userCharmId;
  doc["sleepDisabled"]= in.sleepDisabled;          // <-- NEW

  File f = LittleFS.open(CFG_PATH, "w");
  if (!f) return false;
  bool ok = (serializeJson(doc, f) > 0);
  f.close();
  return ok;
}

bool loadHits(JsonDocument &doc){
  doc.clear();
  if (!LittleFS.exists(HITS_PATH)) {
    saveHits(doc);
    return true;
  }
  File f = LittleFS.open(HITS_PATH, "r");
  if (!f) return false;
  auto e = deserializeJson(doc, f);
  f.close();
  return !e;
}

bool saveHits(const JsonDocument &doc){
  File f = LittleFS.open(HITS_PATH, "w");
  if (!f) return false;
  bool ok = (serializeJson(doc, f) > 0);
  f.close();
  return ok;
}