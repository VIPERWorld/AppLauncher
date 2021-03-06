
// CTK includes
#include <ctkAppLauncherSettings.h>
#include <ctkTest.h>

// Qt includes
#include <QTemporaryFile>

// ----------------------------------------------------------------------------
class ctkAppLauncherSettingsTester: public QObject
{
  Q_OBJECT

private slots:
  void testDefaults();
  void testLauncherDir();
  void testLauncherName();
  void testReadSettingsError();
  void testPathSep();
  void testLibraryPathVariableName();
};

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testLauncherDir()
{
  ctkAppLauncherSettings appLauncherSettings;
  QCOMPARE(appLauncherSettings.launcherDir(), QString());

  appLauncherSettings.setLauncherDir("/path/to");
  QCOMPARE(appLauncherSettings.launcherDir(), QString("/path/to"));
}

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testLauncherName()
{
  ctkAppLauncherSettings appLauncherSettings;
  QCOMPARE(appLauncherSettings.launcherName(), QString());

  appLauncherSettings.setLauncherName("Foo");
  QCOMPARE(appLauncherSettings.launcherName(), QString("Foo"));
}

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testDefaults()
{
}

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testReadSettingsError()
{
  ctkAppLauncherSettings appLauncherSettings;
  QCOMPARE(appLauncherSettings.readSettings(""), false);
  QCOMPARE(appLauncherSettings.readSettingsError(), QString());

  QCOMPARE(appLauncherSettings.readSettings("/path/to/invalid"), false);
  QCOMPARE(appLauncherSettings.readSettingsError(), QString());

  // Disable on windows where setting group permission is not supported
  // See http://stackoverflow.com/questions/5021645/qt-setpermissions-not-setting-permisions
#ifndef Q_OS_WIN32
  QTemporaryFile nonreadable("testReadSettingsError");
  QVERIFY(nonreadable.open());
  QFile file(nonreadable.fileName());
  QVERIFY(file.setPermissions(QFile::WriteOwner));
  QCOMPARE(appLauncherSettings.readSettings(nonreadable.fileName()), false);
  QCOMPARE(appLauncherSettings.readSettingsError(),
        QString("Failed to read launcher setting file [%1]").arg(nonreadable.fileName()));
#endif
}

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testPathSep()
{
  ctkAppLauncherSettings appLauncherSettings;
#if defined(Q_OS_WIN32)
  QCOMPARE(appLauncherSettings.pathSep(), QString(";"));
#else
  QCOMPARE(appLauncherSettings.pathSep(), QString(":"));
#endif
}

// ----------------------------------------------------------------------------
void ctkAppLauncherSettingsTester::testLibraryPathVariableName()
{
  ctkAppLauncherSettings appLauncherSettings;
#if defined(Q_OS_WIN32)
  QCOMPARE(appLauncherSettings.libraryPathVariableName(), QString("PATH"));
#elif defined(Q_OS_MAC)
  QCOMPARE(appLauncherSettings.libraryPathVariableName(), QString("DYLD_LIBRARY_PATH"));
#elif defined(Q_OS_LINUX)
  QCOMPARE(appLauncherSettings.libraryPathVariableName(), QString("LD_LIBRARY_PATH"));
#else
  QFAIL("Platform not yet supported");
#endif
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(ctkAppLauncherSettingsTest)
#include "moc_ctkAppLauncherSettingsTest.cpp"
