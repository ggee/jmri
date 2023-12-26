# This script is for creating an MQTT topic to listen and control the JMRI power state
#
# Author: Gregory Gee

import java
import java.beans
import jmri
import javax.swing.JButton
import apps

from org.python.core.util import StringUtil

mqttAdapter = jmri.InstanceManager.getDefault( jmri.jmrix.mqtt.MqttSystemConnectionMemo ).getMqttAdapter()
topicBase = "jmri/"

# other stuff here
class PowerListener(java.beans.PropertyChangeListener):
  def propertyChange(self, event):
    if powermanager.getPower()== jmri.PowerManager.ON :
      print "Power is on"
      mqttAdapter.publish(topicBase + "power", "ON")

    if powermanager.getPower()== jmri.PowerManager.OFF :
      print "Power is off"
      mqttAdapter.publish(topicBase + "power", "OFF")

class PowerMqttListener(jmri.jmrix.mqtt.MqttEventListener) :
  def notifyMqttMessage(self, topic, message) :
    # all this listener does is print
    print("Received \""+topic+"\" \""+message+"\"")
    if message == 'ON' and powermanager.getPower() == jmri.PowerManager.OFF :
      print "Turn on power"
      powermanager.setPower(jmri.PowerManager.ON)
    if message == 'OFF' and powermanager.getPower()== jmri.PowerManager.ON :
      print "Turn off power"
      powermanager.setPower(jmri.PowerManager.OFF)
	  
#On startup
if powermanager.getPower()== jmri.PowerManager.ON :
  print "Power is on"
  mqttAdapter.publish(topicBase + "power", "ON")

if powermanager.getPower()== jmri.PowerManager.OFF :
  print "Power is off"
  mqttAdapter.publish(topicBase + "power", "OFF")

powerListener = PowerListener()
powerMqttistener = PowerMqttListener()

powermanager.addPropertyChangeListener("power", powerListener)
mqttAdapter.subscribe(topicBase + "power", powerMqttistener)

# Add buttons to UI to start and stop listeners.  You remove these lines if you don't want the UI buttons.
def remove_listeners( event ):
  print "Stopping MQTT Power Listener"
  # Remove the listener from the powermanager. 
  powermanager.removePropertyChangeListener("power", powerListener)
  mqttAdapter.unsubscribe(topicBase + "power", powerMqttistener)

def add_listeners( event ):
  print "Starting MQTT Power Listener"
  # Attach that listener to the powermanager. 
  powermanager.addPropertyChangeListener("power", powerListener)
  mqttAdapter.subscribe(topicBase + "power", powerMqttistener)

b2 = javax.swing.JButton("Start MQTT Power Listen")
b2.actionPerformed = add_listeners
apps.Apps.buttonSpace().add(b2)

b1 = javax.swing.JButton("Stop MQTT Power Listen")
b1.actionPerformed = remove_listeners
apps.Apps.buttonSpace().add(b1)

apps.Apps.buttonSpace().revalidate()

