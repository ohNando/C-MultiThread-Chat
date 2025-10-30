import { useState, useEffect } from 'react';

const WS_URL = 'ws://localhost:8081'; 
const WS_PROTOCOL = 'tcp-chat-protocol';

export const useWebSocket = () => {
    const [messages, setMessages] = useState([]);
    const [ws, setWs] = useState(null);
    const [isConnected, setIsConnected] = useState(false);

    useEffect(() => {
        console.log('WS [DEBUG]: useEffect tetiklendi. Bağlantı kuruluyor...');

        const socket = new WebSocket(WS_URL, WS_PROTOCOL);
        setWs(socket);
        
        console.log('WS [DEBUG]: WebSocket nesnesi oluşturuldu:', socket);

        socket.onopen = () => {
            console.log('WS [DEBUG]: Bağlantı başarılı (onopen). State güncelleniyor.');
            setIsConnected(true);
        };

        socket.onmessage = (event) => {
            console.log('WS [DEBUG]: Mesaj alındı:', event.data);
            setMessages(prev => [...prev, String(event.data)]);
        };

        socket.onclose = (event) => {
            console.log(`WS [DEBUG]: Bağlantı kapandı. Kod: ${event.code}, Sebep: ${event.reason}`);
            setIsConnected(false);
        };

        socket.onerror = (error) => {
            console.error('WS [DEBUG]: Kritik WebSocket Hatası:', error);
        };

        return () => {
            console.log('WS [DEBUG]: useEffect temizleniyor. Socket kapatılıyor.');
            if (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING) {
                 socket.close(1000, 'Component unmount');
            }
        };
    }, []);

    const sendMessage = (username, message) => {
        if(ws && isConnected) {
            const formattedMessage = `/chat [${username}]: ${message}`; 
            ws.send(formattedMessage);
        }else{
            console.warn('WS [DEBUG]: Mesaj gönderilemedi. Bağlantı yok veya socket hazır değil.');
        }
    };

    return { messages, isConnected, sendMessage, setMessages};
};