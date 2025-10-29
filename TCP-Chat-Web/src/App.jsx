// tcp-chat-gui/src/App.jsx (Varsayılan kodu tamamen silin ve bununla değiştirin)

import React, { useState } from 'react';
// Hazırladığımız WebSocket hook'unu import ediyoruz
import { useWebSocket } from './hooks/useWebSocket'; 
import './App.css'; // Stil dosyanızın yolu

const App = () => { 
    // WebSocket hook'undan gerekli durumu ve fonksiyonları çekiyoruz
    const { messages, isConnected, sendMessage } = useWebSocket();
    const [input, setInput] = useState('');

    const handleSend = (e) => { 
        e.preventDefault(); // Formun yenilenmesini engeller
        if (input.trim() && isConnected) {
            // Mesajı WebSocket üzerinden C Backend'ine /chat komutuyla gönder
            sendMessage(input.trim());
            setInput('');
        }
    };

    return (
        // Sohbet uygulamasının ana kapsayıcısı
        <div className="chat-container" style={{ display: 'flex', flexDirection: 'column', height: '100vh', padding: '10px', boxSizing: 'border-box' }}>
            <header style={{ padding: '10px', borderBottom: '1px solid #eee' }}>
                <h1>TCP Chat Köprüsü</h1>
                <p style={{ color: isConnected ? 'green' : 'red', fontWeight: 'bold' }}>
                    Durum: {isConnected ? 'Bağlı (Port 8081)' : 'Bağlantı Yok'}
                </p>
            </header>
            
            {/* Mesaj Görüntüleme Alanı */}
            <div className="messages-box" style={{ overflowY: 'auto', flexGrow: 1, padding: '10px', border: '1px solid #ccc', marginBottom: '10px', backgroundColor: '#f9f9f9' }}>
                {messages.map((msg, index) => (
                    // C backend'inden gelen mesajlar listelenir
                    <p key={index} style={{ margin: '5px 0', wordWrap: 'break-word' }}>
                        {msg.trim()}
                    </p>
                ))}
            </div>

            {/* Mesaj Gönderme Formu */}
            <form onSubmit={handleSend} style={{ display: 'flex' }}>
                <input
                    type="text"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Mesajınızı yazın..."
                    disabled={!isConnected}
                    style={{ flexGrow: 1, padding: '10px', border: '1px solid #ccc' }}
                />
                <button 
                    type="submit" 
                    disabled={!isConnected || !input.trim()} 
                    style={{ padding: '10px 15px', marginLeft: '10px', backgroundColor: isConnected ? '#007bff' : '#ccc', color: 'white', border: 'none', cursor: isConnected ? 'pointer' : 'not-allowed' }}
                >
                    Gönder
                </button>
            </form>
        </div>
    );
};

export default App;