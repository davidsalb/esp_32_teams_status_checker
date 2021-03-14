
const char* ssid = "";
const char* password =  "";

const char* client_id = "";

// TLS certificates:
const char* login_ca = \
                       "-----BEGIN CERTIFICATE-----\n" \
                       "MIIHQzCCBiugAwIBAgIQDHVGe+MQWD43uLVp+2InQDANBgkqhkiG9w0BAQsFADBN\n" \
                       "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMScwJQYDVQQDEx5E\n" \
                       "aWdpQ2VydCBTSEEyIFNlY3VyZSBTZXJ2ZXIgQ0EwHhcNMjAxMDEzMDAwMDAwWhcN\n" \
                       "MjExMDEyMjM1OTU5WjB/MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3Rv\n" \
                       "bjEQMA4GA1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0\n" \
                       "aW9uMSkwJwYDVQQDEyBzdGFtcDIubG9naW4ubWljcm9zb2Z0b25saW5lLmNvbTCC\n" \
                       "ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJ7qLcZKp2HEcxeiDZ8ACbUU\n" \
                       "623p6BDxKS6K6mdeaXVm5qCPllLu+7O4N7i5ky0ZWqdT79dMYpIUXnb/rfmbUuEZ\n" \
                       "bmvF0MsiEP3GRVa2F2w0qP5DRfHqrdR8WEVYqW8eeKruYmLQcQR9/MOCXVpu+B/z\n" \
                       "RFDNrgpDJ2zKxogrHWzgB1NZ/dUlBW/Onxqi7x///cRuSlYWVv7ZlEhvHrkUuT0e\n" \
                       "bd0YmPaA5xM5BWtKzmL054MNMNX14R5O/xRVg8eODzXDAWZoyb1wGpZwdFt/9pFC\n" \
                       "VcAW+TdOofb+QNkPApT33Lj8YV9hmANMhZRd3pL3clQZisHacehPyRUBFTgAuE8C\n" \
                       "AwEAAaOCA+swggPnMB8GA1UdIwQYMBaAFA+AYRyCMWHVLyjnjUY4tCzhxtniMB0G\n" \
                       "A1UdDgQWBBTHq3n2f72liFomqgNCMSl+uQpHiDCCASYGA1UdEQSCAR0wggEZgh1s\n" \
                       "b2dpbi5taWNyb3NvZnRvbmxpbmUtaW50LmNvbYIbbG9naW4ubWljcm9zb2Z0b25s\n" \
                       "aW5lLXAuY29tghlsb2dpbi5taWNyb3NvZnRvbmxpbmUuY29tgh5sb2dpbjIubWlj\n" \
                       "cm9zb2Z0b25saW5lLWludC5jb22CGmxvZ2luMi5taWNyb3NvZnRvbmxpbmUuY29t\n" \
                       "gh9sb2dpbmV4Lm1pY3Jvc29mdG9ubGluZS1pbnQuY29tghtsb2dpbmV4Lm1pY3Jv\n" \
                       "c29mdG9ubGluZS5jb22CJHN0YW1wMi5sb2dpbi5taWNyb3NvZnRvbmxpbmUtaW50\n" \
                       "LmNvbYIgc3RhbXAyLmxvZ2luLm1pY3Jvc29mdG9ubGluZS5jb20wDgYDVR0PAQH/\n" \
                       "BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjBrBgNVHR8EZDBi\n" \
                       "MC+gLaArhilodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vc3NjYS1zaGEyLWc3LmNy\n" \
                       "bDAvoC2gK4YpaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL3NzY2Etc2hhMi1nNy5j\n" \
                       "cmwwTAYDVR0gBEUwQzA3BglghkgBhv1sAQEwKjAoBggrBgEFBQcCARYcaHR0cHM6\n" \
                       "Ly93d3cuZGlnaWNlcnQuY29tL0NQUzAIBgZngQwBAgIwfAYIKwYBBQUHAQEEcDBu\n" \
                       "MCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wRgYIKwYBBQUH\n" \
                       "MAKGOmh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFNIQTJTZWN1\n" \
                       "cmVTZXJ2ZXJDQS5jcnQwDAYDVR0TAQH/BAIwADCCAQMGCisGAQQB1nkCBAIEgfQE\n" \
                       "gfEA7wB2APZclC/RdzAiFFQYCDCUVo7jTRMZM7/fDC8gC8xO8WTjAAABdSPLxZgA\n" \
                       "AAQDAEcwRQIhAJ9Jj6HoiAoXcGojyRuwAdxA8LymL4Q7hVnCJFj8yDKkAiBryXf2\n" \
                       "L+JyaGPKwQObD0PkemkrkZIztfu66hIfWkDhsQB1AFzcQ5L+5qtFRLFemtRW5hA3\n" \
                       "+9X6R9yhc5SyXub2xw7KAAABdSPLxeEAAAQDAEYwRAIgEZNSXl4vJaL07dozlUIC\n" \
                       "SumkPVuFrz0MwgY3tvnYVPMCIFxCX9ncnxUlmBwUHQjsj5V1jldv/kxHztsCpZLa\n" \
                       "nY+/MA0GCSqGSIb3DQEBCwUAA4IBAQDEylg5vLYyVdvBVNwieYMUPs568EAAhkWa\n" \
                       "BsxC93Di3/c2AFSUxLfhCDmEA+RvZIiuUB56NRXn3YhGvJ2yufF5SnS7oKukx62w\n" \
                       "LCzAVa59WOntvsUBw9MLAWHD4x4J4rUIJ0AMIe+L730wuH/M3H34PINz3ECvvXYo\n" \
                       "rMbs/ZRKq03Nq0JcqkQcd/Igo4fV+MxCiNsNb9vUbBcxPm5k4A0pCJ610THXnduw\n" \
                       "srchL16VQroB62uX/zvn4mQum6dWmWaYx6PFLjeyv9lWrsqSHTD9tmsVloaHcmrn\n" \
                       "zl+uyVyLEOxiGXZZppNWcVF/OHS5d2u5iUwWALsL176vfp+FpZtN\n" \
                       "-----END CERTIFICATE-----\n";

const char* graph_ca = \
                       "-----BEGIN CERTIFICATE-----\n" \
                       "MIIITDCCBjSgAwIBAgITMwAFhY3/XQ6Av605zAAAAAWFjTANBgkqhkiG9w0BAQwF\n" \
                       "ADBZMQswCQYDVQQGEwJVUzEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9u\n" \
                       "MSowKAYDVQQDEyFNaWNyb3NvZnQgQXp1cmUgVExTIElzc3VpbmcgQ0EgMDEwHhcN\n" \
                       "MjAxMjE1MTM0OTIzWhcNMjExMjEwMTM0OTIzWjBqMQswCQYDVQQGEwJVUzELMAkG\n" \
                       "A1UECBMCV0ExEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBD\n" \
                       "b3Jwb3JhdGlvbjEcMBoGA1UEAxMTZ3JhcGgubWljcm9zb2Z0LmNvbTCCASIwDQYJ\n" \
                       "KoZIhvcNAQEBBQADggEPADCCAQoCggEBALeHFecfkR7ztUWN0nvDBKPn7hDBEE+7\n" \
                       "JJo3cWWJWZe7633OlzY4FRvvk8++gqdtH5+EgJZy51XlH3mnU3hdcUc6IB+5W7Cv\n" \
                       "GEWSOqLLJEWQ3w7xnU2uvwGA5RrYhSSMvxZFvZYV9pkNXn47ZziRa2SX4JtNKqrC\n" \
                       "OQTPYBminbPOJJRaZB2jKKNeZOSS+tZiyg8s/AYL/ffH+1fkplv5ONi0ur4nr1uE\n" \
                       "EMMr/RyjT1IKrxIKVZEC4lKXblT9QAadw4QwcN1WJmD+JnQgMgh/BUajc4HV86Lm\n" \
                       "KL9jncXQzMD3YyIBc+52Tofpf7HdzB51Hrv263EmBbmrg6NiImbd+2UCAwEAAaOC\n" \
                       "A/owggP2MIIBfQYKKwYBBAHWeQIEAgSCAW0EggFpAWcAdQD2XJQv0XcwIhRUGAgw\n" \
                       "lFaO400TGTO/3wwvIAvMTvFk4wAAAXZmstF2AAAEAwBGMEQCIF+jHRNMQICET35w\n" \
                       "NZM6xb8l6V4e8lnOfBBkGXB3zVmFAiA0sKHQ3Z2oYnIzt8Af9MdFkYqwvGkOT3zf\n" \
                       "GrCBmPXEkwB2AESUZS6w7s6vxEAH2Kj+KMDa5oK+2MsxtT/TM5a1toGoAAABdmay\n" \
                       "0YcAAAQDAEcwRQIgRxD6VkyQ8rEo+oRUBJlxc2N6PmpXJfRQRXlSJEaFFrwCIQDW\n" \
                       "2B4K7AXSDk9ZtY8mjNEC5IS1US99sn1VN5X9/CSPRQB2AFzcQ5L+5qtFRLFemtRW\n" \
                       "5hA3+9X6R9yhc5SyXub2xw7KAAABdmay0aoAAAQDAEcwRQIgRl/94kbcv6tCcSNd\n" \
                       "Gcg7l44ERgW4yEHthGgNV+vZ9uwCIQCnnq2pFL2vClm4rMBzsXPXa6Q5Ot14IgQI\n" \
                       "ry3+CPLlYDAnBgkrBgEEAYI3FQoEGjAYMAoGCCsGAQUFBwMCMAoGCCsGAQUFBwMB\n" \
                       "MDwGCSsGAQQBgjcVBwQvMC0GJSsGAQQBgjcVCIe91xuB5+tGgoGdLo7QDIfw2h1d\n" \
                       "goTlaYLzpz4CAWQCASMwga4GCCsGAQUFBwEBBIGhMIGeMG0GCCsGAQUFBzAChmFo\n" \
                       "dHRwOi8vd3d3Lm1pY3Jvc29mdC5jb20vcGtpb3BzL2NlcnRzL01pY3Jvc29mdCUy\n" \
                       "MEF6dXJlJTIwVExTJTIwSXNzdWluZyUyMENBJTIwMDElMjAtJTIweHNpZ24uY3J0\n" \
                       "MC0GCCsGAQUFBzABhiFodHRwOi8vb25lb2NzcC5taWNyb3NvZnQuY29tL29jc3Aw\n" \
                       "HQYDVR0OBBYEFD0RYKlEfW3vSHmiDNEdqwld5mUzMA4GA1UdDwEB/wQEAwIEsDAe\n" \
                       "BgNVHREEFzAVghNncmFwaC5taWNyb3NvZnQuY29tMGQGA1UdHwRdMFswWaBXoFWG\n" \
                       "U2h0dHA6Ly93d3cubWljcm9zb2Z0LmNvbS9wa2lvcHMvY3JsL01pY3Jvc29mdCUy\n" \
                       "MEF6dXJlJTIwVExTJTIwSXNzdWluZyUyMENBJTIwMDEuY3JsMGYGA1UdIARfMF0w\n" \
                       "UQYMKwYBBAGCN0yDfQEBMEEwPwYIKwYBBQUHAgEWM2h0dHA6Ly93d3cubWljcm9z\n" \
                       "b2Z0LmNvbS9wa2lvcHMvRG9jcy9SZXBvc2l0b3J5Lmh0bTAIBgZngQwBAgIwHwYD\n" \
                       "VR0jBBgwFoAUDyBd16FXlduSzyvQx8J3BM5ygHYwHQYDVR0lBBYwFAYIKwYBBQUH\n" \
                       "AwIGCCsGAQUFBwMBMA0GCSqGSIb3DQEBDAUAA4ICAQCW4qY5UM0YB2SzX82k2AzU\n" \
                       "Yp4KsC3z0sZag1P0tuiaseBibxNSoY6Bsn8JaRCHIre73agz1oktQsF7eyi93gXV\n" \
                       "mk7dzFh4k24icADqxiqy+p/thfhNz/Tr/8YvvUHAKDrjrDfQWJoaJIF6fhiFwJqB\n" \
                       "lhaZ0M0U+I4U2r+6etcgPOrE25KbaNvIa4wT7VxPTepTR5AQiSJtGVH3ZhILn/xl\n" \
                       "/c7c4f8z5Q7QPER8Iwp/fS4i+i+7XG+d0uvN7Bio3e0mEvg0yIkyB5LDdWd7WEih\n" \
                       "mUa6yyxFnerPiW9Nmr6qtBgIpqW93Wvd0YbfqubXhQEVGKViuy/EneLui3PLDBVn\n" \
                       "icsz8Uo2FsT3hnJFr6Ku+aVfLoL9Zf5z9OvHFIagjbS2YgydqQHMP1VXqZtymKrQ\n" \
                       "xmj0jqJxbqyButocPEIkZZ9p+5tOeuBJxurSrsERQhXuzdfgChg8Q7fl6gyYwb45\n" \
                       "ucIkEhn4+4U8K2hWld4Eua58sTqPdTO58aFUYJZyl8Vb4xicP6scRK7ZC7JEjFLs\n" \
                       "87/KQjgNNPUpvifIr7/82EyKreFI8gPJjcYs5WWcX+PdrlHvpgHy4Bo91nT0tFLM\n" \
                       "eKTJSNzQ7xnWjfoklvkJDpCNYV/jHozpmtJzdEY98AraDaS9K2V+WyaZrZnz9Ke1\n" \
                       "eDNlCJodi/w5TEQexdC+LA==\n" \
                       "-----END CERTIFICATE-----\n";
