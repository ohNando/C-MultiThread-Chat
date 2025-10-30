import { useState, useEffect } from 'react';

// C backend'inizin adresi
const WS_URL = 'ws://localhost:8081'; 
const WS_PROTOCOL = 'tcp-chat-protocol';

export const useWebSocket = () => {
    // State tanımlamaları
    const [messages, setMessages] = useState([]);
    const [ws, setWs] = useState(null);
    const [isConnected, setIsConnected] = useState(false);

    useEffect(() => {
        // LOG 1: Hook'un tetiklendiğini kontrol et
        console.log('WS [DEBUG]: useEffect tetiklendi. Bağlantı kuruluyor...');

        // WebSocket nesnesini oluştur
        const socket = new WebSocket(WS_URL, WS_PROTOCOL);
        setWs(socket);
        
        // LOG 2: Socket nesnesinin durumunu kontrol et
        console.log('WS [DEBUG]: WebSocket nesnesi oluşturuldu:', socket);

        // --- Olay Dinleyicileri (Event Listeners) ---

        socket.onopen = () => {
            // LOG 3: Bağlantı BAŞARILI
            console.log('WS [DEBUG]: Bağlantı başarılı (onopen). State güncelleniyor.');
            setIsConnected(true);
        };

        socket.onmessage = (event) => {
            // LOG 4: Mesaj GELDİ
            console.log('WS [DEBUG]: Mesaj alındı:', event.data);
            setMessages(prev => [...prev, String(event.data)]);
        };

        socket.onclose = (event) => {
            // LOG 5: Bağlantı KAPANDI
            console.log(`WS [DEBUG]: Bağlantı kapandı. Kod: ${event.code}, Sebep: ${event.reason}`);
            setIsConnected(false);
        };

        socket.onerror = (error) => {
            // LOG 6: HATA oluştu
            console.error('WS [DEBUG]: Kritik WebSocket Hatası:', error);
        };

        // Temizlik fonksiyonu
        return () => {
            console.log('WS [DEBUG]: useEffect temizleniyor. Socket kapatılıyor.');
            if (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING) {
                 socket.close(1000, 'Component unmount');
            }
        };
    }, []); // Sadece bir kez çalışır

    // Mesaj gönderme fonksiyonu (Kullanıcı adı yönetimi dahil)
    const sendMessage = (username, message) => {
        if(ws && isConnected) {
            // C Backend için format: /chat [KullanıcıAdı]: Mesaj
            const formattedMessage = `/chat [${username}]: ${message}`; 
            ws.send(formattedMessage);
        }else{
            console.warn('WS [DEBUG]: Mesaj gönderilemedi. Bağlantı yok veya socket hazır değil.');
        }
    };

    return { messages, isConnected, sendMessage };
};