// src/App.jsx

import React, { useState, useEffect, useRef } from 'react';
import { useWebSocket } from './hooks/useWebSocket'; 
import './App.css';

const App = () => { 
    const [username, setUsername] = useState(''); 
    const { messages, isConnected, sendMessage, setMessages } = useWebSocket();
    const [input, setInput] = useState('');
    
    const messagesEndRef = useRef(null);

    useEffect(() => {
        messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
    }, [messages]);

    const handleSend = (e) => { 
        e.preventDefault(); 
        const trimmedInput = input.trim();
        if (trimmedInput && isConnected) {
            sendMessage(username, trimmedInput); 
            const localMsg = `[${username}]: ${trimmedInput}`;
            setMessages(prev => [...prev, localMsg]);
            setInput('');
        }
    };
    
    const parseMessage = (msg) => {
        const serverMatch = msg.match(/^SERVER: (.*)/);
        if (serverMatch) {
            return { type: 'server', sender: 'Server', content: serverMatch[1] };
        }

        const chatMatch = msg.match(/^\[(.*?)\]: (.*)/);
        if (chatMatch) {
            const sender = chatMatch[1];
            const content = chatMatch[2];
            const type = (sender === username) ? 'own' : 'other';
            return { type, sender, content };
        }
        
        return { type: 'other', sender: 'Unknown', content: msg }; 
    };

    
    if (!username) {
        return (
            <div className="login-screen">
                <h2>TCP Chat Köprüsü</h2>
                <p className={`status ${isConnected ? 'status-connected' : 'status-disconnected'}`}>
                    Durum: {isConnected ? 'Connected (Port 8081)' : 'Waiting for backend ...'}
                </p>
                <input
                    type="text"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Enter your username"
                    onKeyDown={(e) => { if (e.key === 'Enter' && input.trim()) setUsername(input.trim()); }}
                />
                <button onClick={() => { if (input.trim()) setUsername(input.trim()); }}
                        disabled={!input.trim() || !isConnected}>
                    Sohbete Başla
                </button>
            </div>
        );
    }

    return (
        <div className="chat-container">
            <header className="header">
                <h1>TCP Chat Köprüsü</h1>
                <p className={`status ${isConnected ? 'status-connected' : 'status-disconnected'}`}>
                    Kullanıcı: {username} | Durum: {isConnected ? 'Online' : 'Offline'}
                </p>
            </header>
            
            <div className="messages-box">
                {messages.map((msg, index) => {
                    const parsed = parseMessage(msg.trim());

                    if (parsed.type === 'server') {
                        return (
                            <div key={index} className="message server">
                                {parsed.content}
                            </div>
                        );
                    }

                    return (
                        <div key={index} className={`message ${parsed.type}`}>
                            <div className="message-sender">{parsed.type === 'other' ? parsed.sender : ''}</div>
                            <div className="message-content">
                                {parsed.content}
                            </div>
                        </div>
                    );
                })}
                {}
                <div ref={messagesEndRef} />
            </div>

            <form onSubmit={handleSend} className="input-form">
                <input
                    type="text"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                    placeholder="Enter your message..."
                    disabled={!isConnected}
                />
                <button 
                    type="submit" 
                    disabled={!isConnected || !input.trim()}>
                    Send
                </button>
            </form>
        </div>
    );
};

export default App;