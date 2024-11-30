package com.estebandev.core.utils.display;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.LinkedHashSet;
import java.util.Set;

public class DisplayImpl implements Display {
    private final Set<String> logSet;
    private final BufferedReader reader;
    private final InputStream inputStream;

    public DisplayImpl() {
        this.inputStream = System.in;
        this.logSet = new LinkedHashSet<>();
        this.reader = new BufferedReader(new InputStreamReader(this.inputStream));
    }

    @Override
    public void addLog(String message) {
        this.logSet.add(message);
    }

    @Override
    public String scanLine() {
        try {
            String value = reader.readLine();
            return value;
        } catch (IOException e) {
            return "";
        }
    }

    @Override
    public int scan() {
        return Integer.parseInt(scanLine());
    }

    @Override
    public void show() {
        this.logSet.forEach(message -> {
            System.out.println(message);
        });
        this.logSet.clear();
    }

    @Override
    public void clear() {
        this.logSet.clear();
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }
}
