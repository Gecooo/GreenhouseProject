#include "AlarmDispatcher.h"
//--------------------------------------------------------------------------------------------------------------------------------------
AlarmDispatcher::AlarmDispatcher()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------
void AlarmDispatcher::Alarm(AlertRule* rule)
{
  #ifdef USE_SMS_MODULE
    // сперва ищем - нет ли такой обработанной тревоги или тревоги в очереди
    bool foundInSMSLists = false;
    size_t smsTo = queueSMSAlarms.size();
    for(size_t i=0;i<smsTo;i++)
    {
      if(queueSMSAlarms[i] == rule)
      {
        foundInSMSLists = true;
        break;
      }
    }

    if(!foundInSMSLists)
    {
      smsTo = processedSMSAlarms.size();
      for(size_t i=0;i<smsTo;i++)
      {
        if(processedSMSAlarms[i] == rule)
        {
          foundInSMSLists = true;
          break;
        }
      }
    }

     // если не найдено - просто добавляем в очередь
     if(!foundInSMSLists)
      queueSMSAlarms.push_back(rule);
      
  #else 
      UNUSED(rule);
  #endif  
}
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_SMS_MODULE
//--------------------------------------------------------------------------------------------------------------------------------------
void AlarmDispatcher::MarkSMSAlarmDone()
{
  if(!queueSMSAlarms.size())
    return;

   AlertRule* r = queueSMSAlarms[queueSMSAlarms.size()-1];
   processedSMSAlarms.push_back(r);
   queueSMSAlarms.pop();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void AlarmDispatcher::ClearProcessedAlarms()
{
  #if defined(USE_ALARM_DISPATCHER) && defined(USE_SMS_MODULE) && defined(CLEAR_ALARM_STATUS)
    processedSMSAlarms.Clear();
  #endif
}
//--------------------------------------------------------------------------------------------------------------------------------------
String AlarmDispatcher::GetSMSAlarmData()
{
  String result;

  if(!queueSMSAlarms.size())
    return result;

  AlertRule* r = queueSMSAlarms[queueSMSAlarms.size()-1];

  result = ALARM_SMS_TEXT;
  result += r->GetName();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
#endif // USE_SMS_MODULE
//--------------------------------------------------------------------------------------------------------------------------------------

