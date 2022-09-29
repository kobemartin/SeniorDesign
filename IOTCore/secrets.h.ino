#include <pgmspace.h>

#define SECRET
#define THINGNAME "MyNewESP32"

const char WIFI_SSID[] = "Kobe";
const char WIFI_PASSWORD[] = "much0076";
const char AWS_IOT_ENDPOINT[] = "a2bnf9w497tv0a-ats.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";
