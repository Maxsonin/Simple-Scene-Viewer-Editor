#pragma once

class Application;

class ApplicationSettings
{
private:
    bool m_isVSyncEnabled = false;
    bool m_isPolygonModeFill = true;

    unsigned int samplesPerFragment = 8;
    bool MSAAChanged = false;

public:
    Application* application;

    ApplicationSettings(Application* application)
        : application(application)
    { }

    bool isVSyncEnabled() { return m_isVSyncEnabled; }
    bool isPolygonModeFill() { return m_isPolygonModeFill; }
    void setVSyncEnabled(bool newVal) { m_isVSyncEnabled = newVal; }
    void setPolygonModeFill(bool newVal) { m_isPolygonModeFill = newVal; }

    bool isMSAAChanged() { return MSAAChanged; }
    void setMSAAChanged(bool newVal) { MSAAChanged = newVal; }

    unsigned int getSamplesPerFragment() { return samplesPerFragment; }
    void setSamplesPerFragment(unsigned int newVal) { samplesPerFragment = newVal; }
};
