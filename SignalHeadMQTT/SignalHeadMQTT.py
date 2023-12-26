import java
import java.beans
import jmri
import apps

mqttAdapter = jmri.InstanceManager.getDefault( jmri.jmrix.mqtt.MqttSystemConnectionMemo ).getMqttAdapter()
mqttSignalTopic = mqttAdapter.getOptionState("14")

class SignalHeadChanger( java.beans.PropertyChangeListener ):

  def propertyChange( self, event ):
    if (event.propertyName == "Appearance") :
      signalHead = event.source
      mqttAdapter.publish(mqttSignalTopic + signalHead.getSystemName(), signalHead.getAppearanceName())

listener = SignalHeadChanger( )
for signal in signals.getNamedBeanSet():
  signal.addPropertyChangeListener(listener)
