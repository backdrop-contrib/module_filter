<?php

/**
 * @file
 *
 * @author greenSkin
 */

/*******************************************************************************
 * Callback Functions, Forms, and Tables
 ******************************************************************************/

/**
 * Settings form for module filter.
 */
function module_filter_settings() {
  $config = config('module_filter.settings');

  $form['#config'] = 'module_filter.settings';
  $form['set_focus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set focus to filter field on page load'),
    '#description' => t('Currently has no effect when using Overlay module.'),
    '#default_value' => $config->get('set_focus'),
  );

  $form['use_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enhance the modules page with tabs'),
    '#description' => t('Alternate tabbed theme that restructures packages into tabs.'),
    '#default_value' => $config->get('use_tabs'),
  );
  $form['tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tabs'),
    '#description' => t('Settings used with the tabs view of the modules page.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tabs']['count_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Number of enabled modules'),
    '#description' => t('Display the number of enabled modules in the active tab along with the total number of modules.'),
    '#default_value' => $config->get('count_enabled'),
  );
  $form['tabs']['visual_aid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visual aids'),
    '#description' => t('When enabling/disabling modules, the module name will display in the tab summary.<br />When filtering, a count of results for each tab will be presented.'),
    '#default_value' => $config->get('visual_aid'),
  );
  $form['tabs']['hide_empty_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide tabs with no results'),
    '#description' => t('When a filter returns no results for a tab, the tab is hidden. This is dependent on visual aids being enabled.'),
    '#default_value' => $config->get('hide_empty_tabs'),
  );
  $form['tabs']['dynamic_save_position'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dynamically position Save button'),
    '#description' => t("For sites with lots of tabs, enable to help keep the 'Save configuration' button more accessible."),
    '#default_value' => $config->get('dynamic_save_position'),
  );
  $form['tabs']['use_url_fragment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use URL fragment'),
    '#description' => t('Use URL fragment when navigating between tabs. This lets you use the browsers back/forward buttons to navigate through the tabs you selected.') . '<br />' . t('When the Overlay module is enabled this functionality will not be used since overlay relies on the URL fragment.'),
    '#default_value' => $config->get('use_url_fragment'),
  );
  $form['tabs']['use_switch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use switch instead of checkbox'),
    '#description' => t('This is purely cosmetic (at least for now). Displays a ON/OFF switch rather than a checkbox to enable/disable modules.<br /><strong>Modules will not actually be enabled/disabled until the form is saved.</strong>'),
    '#default_value' => $config->get('use_switch'),
  );
  $form['tabs']['track_recent_modules'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track recently enabled/disabled modules'),
    '#description' => t('Adds a "Recent" tab that displays modules that have been enabled or disabled with the last week.'),
    '#default_value' => $config->get('track_recent_modules'),
  );
  $form['tabs']['remember_active_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember active tab.'),
    '#description' => t('When enabled, the active tab will be remembered.'),
    '#default_value' => $config->get('remember_active_tab'),
  );
  $form['tabs']['version_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Place version in own column'),
    '#description' => t("Moves the version out of the description and into it's own column"),
    '#default_value' => $config->get('version_column'),
  );
  $form['tabs']['expanded_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expand description by default'),
    '#description' => t('When enabled, the description will be expanded by default.'),
    '#default_value' => $config->get('expanded_description'),
  );
  $form['tabs']['show_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show module path in modules list'),
    '#description' => t('If enabled, the relative path of each module will display in its row.'),
    '#default_value' => $config->get('show_path'),
  );

  $form['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update status'),
    '#collapsible' => TRUE,
    '#collapsed' => (module_exists('update')) ? FALSE : TRUE,
  );
  $form['update']['update_status_alter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enhance Update Status Report'),
    '#description' => t('Allow module filter module to make enhancements to the update status report.'),
    '#default_value' => $config->get('update_status_alter'),
  );
  $form['update']['remember_update_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember the last selected filter.'),
    '#description' => t('When enabled, the last state (All, Update available, Security update, Unknown) will be remembered.'),
    '#default_value' => $config->get('remember_update_state'),
  );

  if (module_exists('page_actions')) {
    $form['tabs']['dynamic_save_position']['#description'] .= '<br />' . t('The module %name is enabled and thus this setting will have no affect.', array('%name' => t('Page actions')));
  }

  return system_settings_form($form);
}
