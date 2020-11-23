# neopixel_ws

file code của arduino led

- làm nhiệm vụ kết nối vào wifi của raspberry pi
- sau đó kết nối vào websocket server ở `websocket/server.js`
- lắng nghe server gửi màu và dịch json thành màu tương ứng và update vào led, display màu thông qua thư viện của led(neopixel)
