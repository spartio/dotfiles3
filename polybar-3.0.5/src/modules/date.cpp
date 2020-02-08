#include "modules/date.hpp"
#include "drawtypes/label.hpp"

#include "modules/meta/base.inl"

POLYBAR_NS

namespace modules {
  template class module<date_module>;

  date_module::date_module(const bar_settings& bar, string name_) : timer_module<date_module>(bar, move(name_)) {
    if (!m_bar.locale.empty()) {
      setlocale(LC_TIME, m_bar.locale.c_str());
    }

    m_dateformat = string_util::trim(m_conf.get(name(), "date", ""s), '"');
    m_dateformat_alt = string_util::trim(m_conf.get(name(), "date-alt", ""s), '"');
    m_timeformat = string_util::trim(m_conf.get(name(), "time", ""s), '"');
    m_timeformat_alt = string_util::trim(m_conf.get(name(), "time-alt", ""s), '"');

    if (m_dateformat.empty() && m_timeformat.empty()) {
      throw module_error("No date or time format specified");
    }

    m_interval = m_conf.get<decltype(m_interval)>(name(), "interval", 1s);

    m_formatter->add(DEFAULT_FORMAT, TAG_LABEL, {TAG_LABEL, TAG_DATE});

    if (m_formatter->has(TAG_DATE)) {
      m_log.warn("%s: The format tag `<date>` is deprecated, use `<label>` instead.", name());

      m_formatter->get(DEFAULT_FORMAT)->value =
          string_util::replace_all(m_formatter->get(DEFAULT_FORMAT)->value, TAG_DATE, TAG_LABEL);
    }

    if (m_formatter->has(TAG_LABEL)) {
      m_label = load_optional_label(m_conf, name(), "label", "%date%");
    }
  }

  bool date_module::update() {
    auto time = std::time(nullptr);

    auto date_format = m_toggled ? m_dateformat_alt : m_dateformat;
    char date_buffer[64]{'\0'};
    strftime(date_buffer, sizeof(date_buffer), date_format.c_str(), localtime(&time));

    auto time_format = m_toggled ? m_timeformat_alt : m_timeformat;
    char time_buffer[64]{'\0'};
    strftime(time_buffer, sizeof(time_buffer), time_format.c_str(), localtime(&time));

    bool date_changed{strncmp(date_buffer, m_date.c_str(), sizeof(date_buffer)) != 0};
    bool time_changed{strncmp(time_buffer, m_time.c_str(), sizeof(time_buffer)) != 0};

    if (!date_changed && !time_changed) {
      return false;
    }

    m_date = string{date_buffer};
    m_time = string{time_buffer};

    if (m_label) {
      m_label->reset_tokens();
      m_label->replace_token("%date%", m_date);
      m_label->replace_token("%time%", m_time);
    }

    return true;
  }

  bool date_module::build(builder* builder, const string& tag) const {
    if (tag == TAG_LABEL) {
      if (!m_dateformat_alt.empty() || !m_timeformat_alt.empty()) {
        builder->cmd(mousebtn::LEFT, EVENT_TOGGLE);
        builder->node(m_label);
        builder->cmd_close();
      } else {
        builder->node(m_label);
      }
    } else {
      return false;
    }

    return true;
  }

  bool date_module::input(string&& cmd) {
    if (cmd != EVENT_TOGGLE) {
      return false;
    }
    m_toggled = !m_toggled;
    wakeup();
    return true;
  }
}

POLYBAR_NS_END
