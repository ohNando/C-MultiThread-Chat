// tcp-chat-gui/src/App.jsx

import React, { useState } from 'react';
import { useWebSocket } from './hooks/useWebSocket'; 
import './App.css'; 

const App = () => { 
    // State'ler
    const [username, setUsername] = useState(''); 
    const { messages, isConnected, sendMessage } = useWebSocket();
    const [input, setInput] = useState('');

    const handleSend = (e) => { 
        e.preventDefault(); 
        if (input.trim() && isConnected) {
            // Mesajı, kullanıcı adıyla birlikte gönder
            sendMessage(username, input.trim()); 
            setInput('');
        }
    };
    
    // Kullanıcı Adı Giriş Ekranı
    if (!username) {
        return (
            <div className="login-screen" style={{ padding: '50px', textAlign: 'center', height: '100vh', display: 'flex', flexDirection: 'column', justifyContent: 'center', backgroundColor: '#f0f2f5' }}>
                <h2>TCP Chat Köprüsü</h2>
                <p style={{ color: isConnected ? 'green' : 'red', fontWeight: 'bold' }}>
                    Durum: {isConnected ? 'Bağlı' : 'Backend Bekleniyor...'}
                </p>
                <input
                    type="text"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Kullanıcı Adınızı Girin"
                    style={{ margin: '15px auto', padding: '12px', width: '80%', maxWidth: '300px', border: '1px solid #ccc', borderRadius: '5px' }}
                />
                <button onClick={() => { if (input.trim()) setUsername(input.trim()); setInput(''); }}
                        disabled={!input.trim()}
                        style={{ padding: '12px 25px', backgroundColor: '#007bff', color: 'white', border: 'none', borderRadius: '5px', cursor: 'pointer', fontWeight: 'bold' }}>
                    Sohbete Başla
                </button>
            </div>
        );
    }

    // Sohbet Arayüzü
    return (
        <div className="chat-container" style={{ display: 'flex', flexDirection: 'column', height: '100vh', padding: '10px', boxSizing: 'border-box' }}>
            <header style={{ padding: '10px', borderBottom: '2px solid #007bff', backgroundColor: '#e9f7ff' }}>
                <h1>TCP Chat Köprüsü</h1>
                <p style={{ color: isConnected ? 'green' : 'red', fontWeight: 'bold', fontSize: '14px' }}>
                    Bağlı Kullanıcı: {username} | Durum: {isConnected ? 'Çevrimiçi' : 'Bağlantı Yok'}
                </p>
            </header>
            
            {/* Mesaj Görüntüleme Alanı */}
            <div className="messages-box" style={{ overflowY: 'auto', flexGrow: 1, padding: '10px', border: '1px solid #ccc', marginBottom: '10px', backgroundColor: '#fff' }}>
                {messages.map((msg, index) => {
                    const isOwnMessage = msg.startsWith(`[${username}]`);
                    return (
                        <p key={index} style={{ margin: '8px 0', textAlign: isOwnMessage ? 'right' : 'left', wordWrap: 'break-word', padding: '5px' }}>
                            <span style={{ 
                                backgroundColor: isOwnMessage ? '#dcf8c6' : '#fff', 
                                padding: '8px 12px', 
                                borderRadius: '15px', 
                                display: 'inline-block',
                                border: '1px solid #eee'
                            }}>
                                {msg.trim()}
                            </span>
                        </p>
                    );
                })}
            </div>

            {/* Mesaj Gönderme Formu */}
            <form onSubmit={handleSend} style={{ display: 'flex' }}>
                <input
                    type="text"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Mesajınızı yazın..."
                    disabled={!isConnected}
                    style={{ flexGrow: 1, padding: '10px', border: '1px solid #007bff', borderRadius: '5px 0 0 5px' }}
                />
                <button 
                    type="submit" 
                    disabled={!isConnected || !input.trim()} 
                    style={{ padding: '10px 15px', backgroundColor: '#007bff', color: 'white', border: 'none', cursor: 'pointer', borderRadius: '0 5px 5px 0', fontWeight: 'bold' }}
                >
                    Gönder
                </button>
            </form>
        </div>
    );
};

export default App;