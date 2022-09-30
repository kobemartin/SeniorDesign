#include <pgmspace.h>

#define SECRET
#define THINGNAME "MyNewESP32"

const char WIFI_SSID[] = "Kobe";
const char WIFI_PASSWORD[] = "much0076";
const char AWS_IOT_ENDPOINT[] = "a2bnf9w497tv0a-ats.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAMcCYZ/4lHB2VJ1hhFPZmxGXevPYMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMjA5MjkxNzA1
MDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCuJtHJfG6qMq/rut1K
5/+f9X5pZcLj0AoEZPzuGVt4SK4NXuw/F6FJW+7l7gA9NvMB/9YEfGVutHe4rOyP
C3/caYEGwNCmIFLHZ8AUohw8HcZvrfwiDIXFu2SV/N0KeDjI/WNXQeLFKab+4rfr
yy8HQWKK+L0fELqvUNum4vVFo+tU7tL2ac5s9BWHSncCMe1iKj21M0Zxa/cWNTqs
/37CXjtHPt9GWr5Ma8LU0ZFOv5PMMUCSa5emxXLMSMnKqlm0HHfGOzEeyQ06/NIb
OvvgipCVV/Sp874zh/oiLGghRdSF4/FgvY0/lyNt4wrgUrYw/cHuEIyfLMXQFYV9
dSXNAgMBAAGjYDBeMB8GA1UdIwQYMBaAFAnICM/TWJcpiitWX2OKqt45If+OMB0G
A1UdDgQWBBSpISalsuBrONxc/5oYZa5GR0DGgzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAS9tT9yfT3jKbyIrSUGhxrRY/
rdIxZ/qD9Aktgm8jKkzEtab/D/b6k8vRlol2qnFP03GROXGSSyLm4CMDcg61vBsV
ppzL8Lv+1bhV0RBW7UoA2KIEAJI7BbEjVhZYB+XZtkG8FVgAVLmui9tUhUYt1aGm
dVOskcSwWR+DD9KvRO9PqsKtadPfWDU7STTxYdm0NbQnqEkXwRO8LA2+QuJzdooo
pM7Ew6d89BmgtQTfqIiLS9z51q2u8TkhFjlIVTh7JzrR82rzBG6jneWx5HyagFmQ
nios+CxOjq5d6xojyOWTsgZDlIR/CAr0YPrDWeMXP3Rp3CeHJBKO4TL3F3QKuw==
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAribRyXxuqjKv67rdSuf/n/V+aWXC49AKBGT87hlbeEiuDV7s
PxehSVvu5e4APTbzAf/WBHxlbrR3uKzsjwt/3GmBBsDQpiBSx2fAFKIcPB3Gb638
IgyFxbtklfzdCng4yP1jV0HixSmm/uK368svB0Fiivi9HxC6r1DbpuL1RaPrVO7S
9mnObPQVh0p3AjHtYio9tTNGcWv3FjU6rP9+wl47Rz7fRlq+TGvC1NGRTr+TzDFA
kmuXpsVyzEjJyqpZtBx3xjsxHskNOvzSGzr74IqQlVf0qfO+M4f6IixoIUXUhePx
YL2NP5cjbeMK4FK2MP3B7hCMnyzF0BWFfXUlzQIDAQABAoIBAGnPpklAdFCuyVp1
x/y6JwN+elb5gEdAYMW4wkC8YWoc5ruy/+F5rwJOfEFxsO2AlHeThOpm1o5D0jX/
wx9Kgl5eMAn1XxCAtSr466jdskAIa0fxsy7Q4vp32C6VW4v/7aiY5CHhIzJeE1vs
TolSg3lqDcCYTvgwBu18OGkWnqW3aSy/0m9oTGdt7a3NcS0q0Zv+taMwoQrSPMrF
Hl5WLBwlbW7Yf46mbtA8uQcThi3mIn3/onhkh8eeThNNxwlHA9gNJjHvg4M+nAnk
umJyf1zimFT73qD+q0hyImYj4NnToAmvDLFEV3YHILonNENqQEI8jk4tbDPnOThc
1w6484ECgYEA1Zdo/F7bs6FN0HAnehEbYEDx9xrGKO1XVjyZDJNHLH5Gaz1TD4GL
Sly5aNq8V5tAUSr4owd4CVE07uEz/FCLhrZx4XrJARs9mG00AiKpu0SZc7TmnlsL
ZG7DOqHzz2WfzEfKF1qv7tTGKaQSpHWRv/pnqrFLHXIdoJ4RWNm/oW0CgYEA0Lq7
i7KzT6AGpc+1bHGp3MzeO3f73y0z1MjA6j6yWInqejcNxh4mKl0pUE1pysIOloJf
oWtaNvFQrSgw0yqa5gBT+q2Rlz2rXK28pZHqMzioXBwLqDjfioAaaQce9RuroFpX
6mpxeIWVULollTXbumnBsiD3l3PB1Affi44TOeECgYEAk+Gr96e5l4oj9R/hxTsY
6DCvFrY74tRI9tUn5jQGQEEBgnhIDOFVTlwapwVjB4jfJBOLaFzgeBr4ejgEJTqj
D9c7y7CWgKSapfFd4+iZ+aWKVTk3vwYsQhIKK4s74ilrVSWTP04zpzk8JlKJ8cHQ
8sltgxoTdsSuzKkEO68vZZUCgYBe7e5J9LYUaqVbWAIdFfy5DPutdD47HCXMipki
b+1Q0ziu6Mb86c1VyxT1DqAktOlJjnXLxjU7c7jLZ9H82A2muVN4WOQ0Mrny5pk7
fAdV0FjRu3eZxHP2W5inV7O+qhz+SthdJpgAxlBIbMCXAe2y9FJBnzi7ysvGkOYc
ZDteQQKBgQC+UoK67N7Ml+8lO1uuBNEwR7tM2tKA2f1TubI2iTmTob/pX3EZAvcM
+lCh60W2niKBt/gjbk5NuKMcpCG+ZQgeYXWpCk0z2DO/E36UzPyb2tVktT+N5ZgR
WRVH+evoHlKlAyteItuLOZokXLiR5rUw4v9hEbJiszIVLS9/6HzKtg==
-----END RSA PRIVATE KEY-----
)KEY";