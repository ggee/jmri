import sys
import time
import java
import java.beans
import jmri
import javax.swing.JButton
import apps

# 0  = Dark
# 1  = Red
# 2  = Flashing Red
# 4  = Yellow
# 8  = Flashing Yellow
# 16 = Green
# 32 = FLashing Green

mqttAdapter = jmri.InstanceManager.getDefault( jmri.jmrix.mqtt.MqttSystemConnectionMemo ).getMqttAdapter()
mqttSignalTopic = mqttAdapter.getOptionState("14")

#for x in mqttAdapter.options:
#  print "Index: " + x
#  print "   Display name: " + mqttAdapter.getOptionDisplayName(x)
#  print "   State: " + mqttAdapter.getOptionState(x)


class SignalHeadChanger( java.beans.PropertyChangeListener ):

  # when a property changes
  def propertyChange( self, event ):
    if (event.propertyName == "Appearance") :
      signalHead = event.source
      mqttAdapter.publish(mqttSignalTopic + signalHead.getSystemName(), signalHead.getAppearanceName())

# On statup
listener = SignalHeadChanger( )
for signal in signals.getNamedBeanSet():
  signal.addPropertyChangeListener(listener)

# Add some UI buttons to enable/disable.  This code is optional.
def remove_listeners( event ):
  # get the list and add a listener to each
  for signal in signals.getNamedBeanSet():
    signal.removePropertyChangeListener(listener)

def add_listeners( event ):
  # get the list and add a listener to each
  for signal in signals.getNamedBeanSet():
    signal.addPropertyChangeListener(listener)

b2 = javax.swing.JButton("Start SignalHead listen")
b2.actionPerformed = add_listeners
apps.Apps.buttonSpace().add(b2)

b1 = javax.swing.JButton("Stop SignalHead listen")
b1.actionPerformed = remove_listeners
apps.Apps.buttonSpace().add(b1)

apps.Apps.buttonSpace().revalidate()

