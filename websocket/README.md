# websocket

Folder chứa websocket server và các script điều khiển led thông qua websocket

## [server.js](https://github.com/violeine/mooled/blob/main/websocket/server.js)

File khởi tạo http và websocket server(gw.wlan:3001)
- đóng vai trò kết nối arduino led và raspberry pi thông qua websocket
- điều khiển arduino led thông qua websocket và http request bằng web-interface


## Các script điều khiển arduino-led

### [client.js](https://github.com/violeine/mooled/blob/main/websocket/client.js)

script kết nối với websocket và gửi màu và độ sáng cho led thông qua commandline/terminal

```
node client.js -A "["#FCFCFC", "#FFFFFF"]" // nhận 1 mảng màu và lặp vô tận cho tới khi bị kết thúc bởi os 

node client.js -S "#FFAACC" // nhận 1 màu và gửi cho led, kết thúc lập tức

node client.js -B 100 // nhận 1 số (0-255) và gửi cho led, kết thúc lập tức

node client.js -S "#FA2020" -B 50 // có thể kết hợp -S và -B với nhau
```

### [bright-loop-example.sh](https://github.com/violeine/mooled/blob/main/websocket/bright-loop-example.sh)

Bởi vì `client.js` chỉ là 1 commandline script nên ta có thể kết hợp với bash/python/node.js hoặc các scripting language khác để thêm chức năng cho led

Ví dụ như script này loop qua hết các độ sáng và gửi cho led

### [spawn.js](https://github.com/violeine/mooled/blob/main/websocket/spawn.js)

Sử dụng thư viện [chroma.js](https://gka.github.io/chroma.js/) sinh ra mảng 400 màu và gửi cho client.js tạo gradient effect

### [weather.sh](https://github.com/violeine/mooled/blob/main/websocket/weather.sh) 

Curl request tới wttr.in và gửi màu cho led dựa trên thời tiêt tại địa điểm đó
```
./weather.sh hochiminh
```

### Điều khiển màu theo giờ

Có thể dễ dàng đạt được với cronjob và client.js

```
#Chạy vào lúc 6h
0 6 * * *  client.js -S "#D97706" -B 100
#Chạy vào lúc 17h 
0 17 * * * client.js -S "#6D28D9" -B 200
#Mỗi 30 phút cập nhật thời tiết tại hochiminh
*/30 * * * * ./weather.sh hochiminh
```

## [HTTP request](https://github.com/violeine/mooled/blob/28dbac7c8d3c04b3867ab4bbe5a86eff4c3701fb/websocket/server.js#L24)

Các script trên đều được map với 1 post request tương ứng

- POST/array: nhận các bước màu của gradient và chuyển thành 400 màu rổi truyền vào `client.js`

    ```
    curl --location --request POST 'http://gw.wlan:3001/array' \
     --header 'Content-Type: application/json' \
     --data-raw '["#00ffff","#ff00ff"]'
    ```
- POST/stop: nhận 1 màu và độ sáng, và kill tất cả các script đang chạy
    ```
    curl --location --request POST 'http://gw.wlan:3001/stop' \
    --header 'Content-Type: application/json' \
    --data-raw '{
    "color":"#FA2020",
    "brightness":69
    }'
    ```
- POST/weather: nhận 1 địa điểm và chạy script `weather.sh`
    ```
    curl --location --request POST 'http://gw.wlan:3001/weather' \
    --header 'Content-Type: application/json' \
    --data-raw '{"location":"HoChiMinh"}'
    ```

## Resources

- [sinh 1 process bằng node.js](https://nodejs.org/api/child_process.html#child_process_child_process_exec_command_options_callback)
- [minimist: commandline parser](https://github.com/substack/minimist)
- [chroma.js: colors brewer](https://gka.github.io/chroma.js/)
- [wttr.in: weather report](http://wttr.in)
- [crontab timer](https://crontab.guru/)
