import { useState, useEffect } from 'react';

const WS_URL = 'ws://localhost:8081';

export const useWebSocket = () => {
    const [messages, seMessages] = useState([]);
    const [ws, setWs] = useState(null);
    const [isConnected, setIsConnected] = useState(false);

    useEffect(() => {
        const socket = new WebSocket(WS_URL);
        setWs(socket);

        socket.onopen = () => {
            console.log('WebSocket connected to C backend on 8081');
            setIsConnected(true);
        };

        socket.onmessage = (event) => {
            console.log('Message received from C backend:', event.data);
            setMessages(prev => [...prev, event.data]);
        };

        socket.onclose = () => {
            console.log('WebSocket disconnected from C backend');
            setIsConnected(false);
        };

        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
            setMessages(prev => [...prev, 'Server: Connection Error']);
        };

        return () => {
            socket.close();
        };
    }, []);

    const sendMessage = (message) => {
        if(ws && isConnected) {
            const formattedMessage = `/chat: ${message}`;
            ws.send(formattedMessage);
        }else{
            console.warn('WebSocket is not connected. Cannot send message.');
        }
    };

    return { messages,  isConnected, sendMessage };
};