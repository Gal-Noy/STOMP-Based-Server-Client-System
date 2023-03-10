package bgu.spl.net.impl.stomp.database;

import java.util.HashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class User {
    private final String username;
    private final String passcode;
    private volatile boolean connected = true;
    private ReadWriteLock userLock;

    // key: subscription id, value: channel name
    private final HashMap<String, String> subscriptions;
    private AtomicInteger connectionId;

    public User(String _username, String _passcode, int _connectionId){
        this.username = _username;
        this.passcode = _passcode;
        this.connectionId = new AtomicInteger(_connectionId);
        this.subscriptions = new HashMap<>();
        this.userLock = new ReentrantReadWriteLock();
    }

    public String getPasscode(){
        return passcode;
    }

    public boolean getConnected(){
        return connected;
    }

    public void disconnect(){
        connected = false;
    }

    public void connect(){
        connected = true;
    }

    public int getConnectionId(){
        return connectionId.get();
    }

    public void setConnectionId(int _connectionId){
        this.connectionId.compareAndSet(connectionId.get(), _connectionId);
    }

    public void addSubscription(String subscriptionId, String channel){
        subscriptions.put(subscriptionId, channel);
    }

    public void removeSubscription(String subscriptionId){
        subscriptions.remove(subscriptionId);
    }

    public HashMap<String, String> getSubscriptions(){
        return subscriptions;
    }

    public String getSubscriptionId(String channel){
        for (String id : subscriptions.keySet())
            if (subscriptions.get(id).equals(channel))
                return id;
        return null;
    }

    public void clearSubscriptions(){
        subscriptions.clear();
    }

    public void lock(boolean write){
        if (write)
            this.userLock.writeLock().lock();
        else
            this.userLock.readLock().lock();
    }

    public void unlock(boolean write){
        if (write)
            this.userLock.writeLock().unlock();
        else
            this.userLock.readLock().unlock();

    }
}
