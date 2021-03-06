package cn.sehzh;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * 生产者和消费者 （单个生产者单个消费者单个缓冲区）
 * 使用  ArrayBlockingQueue 实现
 */
public class Demo3 {
    public static BlockingQueue<Integer> blockingQueue = new ArrayBlockingQueue<>(1);


    public static void main(String[] args) {
        Thread producer = new Thread(new Producer());
        Thread consumer = new Thread(new Consumer());

        producer.start();
        consumer.start();
    }

    static class Producer implements Runnable {

        @Override
        public void run() {
            while (true) {
                try {
                    int item = (int) (Math.random() * 100);
                    blockingQueue.put(item);

                    System.out.println("Produce: " + item);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    static class Consumer implements Runnable {

        @Override
        public void run() {

            while (true) {
                try {
                    Integer item = blockingQueue.take();

                    System.out.println("Consume: " + item);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

    }
}


