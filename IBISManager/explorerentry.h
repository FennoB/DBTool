#ifndef EXPLORERENTRY_H
#define EXPLORERENTRY_H

#include <QObject>

class ExplorerEntry : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int _hid READ _hid WRITE setHid NOTIFY hidChanged)
  Q_PROPERTY(QString _text READ _text WRITE setText NOTIFY textChanged)
  Q_PROPERTY(QString _source READ _source WRITE setSource NOTIFY symbolSourceChanged)
  Q_PROPERTY(int _layer READ _layer WRITE setLayer NOTIFY layerChanged)
  Q_PROPERTY(bool _visible READ _visible WRITE setVisible NOTIFY visibleChanged)
  Q_PROPERTY(bool _open READ _open WRITE setOpen NOTIFY openChanged)
  Q_PROPERTY(QString _lfnr READ _lfnr NOTIFY lfnrChanged)

  QString m_text;

  QString m_source;

  int m_layer;

  bool m_visible;

  bool m_open;

  int m_hid;

  QString m_lfnr;

public:

  bool lastVisibility;

  ExplorerEntry(const ExplorerEntry&)
  {
    lastVisibility = true;
  }

  ExplorerEntry()
  {

  }

  ExplorerEntry(QString text, QString symbolSource, int layer, bool visible, bool open, int hid, QString lfnr)
  {
    m_text = text;
    m_source = symbolSource;
    m_layer = layer;
    m_visible = visible;
    m_open = open;
    m_hid = hid;
    m_lfnr = lfnr;
  }
  virtual ~ExplorerEntry() {}
  QString _text() const
  {
    return m_text;
  }
  QString _source() const
  {
    return m_source;
  }

  int _layer() const
  {
    return m_layer;
  }

  bool _visible() const
  {
    return m_visible;
  }

  bool _open() const
  {
    return m_open;
  }

  int _hid() const
  {
    return m_hid;
  }

  QString _lfnr() const
  {
    return m_lfnr;
  }

public slots:
  void setText(QString _text)
  {
    if (m_text == _text)
      return;

    m_text = _text;
    emit textChanged(m_text);
  }

  void setSource(QString _source)
  {
    if (m_source == _source)
      return;

    m_source = _source;
    emit symbolSourceChanged(m_source);
  }

  void setLayer(int _layer)
  {
    if (m_layer == _layer)
      return;

    m_layer = _layer;
    emit layerChanged(m_layer);
  }

  void setVisible(bool _visible)
  {
    if (m_visible == _visible)
      return;

    m_visible = _visible;
    emit visibleChanged(m_visible);
  }

  void setOpen(bool _open)
  {
    if (m_open == _open)
      return;

    m_open = _open;
    emit openChanged(m_open);
  }

  void setHid(int hid)
  {
    if (m_hid == hid)
      return;

    m_hid = hid;
    emit hidChanged(m_hid);
  }

signals:
  void textChanged(QString text);
  void symbolSourceChanged(QString symbolSource);
  void layerChanged(int layer);
  void visibleChanged(bool visible);
  void openChanged(bool open);
  void hidChanged(int hid);
  void lfnrChanged(QString _lfnr);
};


#endif // EXPLORERENTRY_H
