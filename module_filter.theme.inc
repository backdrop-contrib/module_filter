<?php

/**
 * @file
 * File for theme functions.
 *
 * @author greenSkin
 */

/**
 * Function theme_module_filter().
 */
function theme_module_filter($variables) {
  $element = $variables['element'];
  return '<div class="module-filter-inputs-wrapper">' . backdrop_render_children($element) . '</div>';
}

/**
 * Theme callback for the modules tabbed form.
 */
function theme_module_filter_system_modules_tabs($variables) {
  $config = config('module_filter.settings');
  if (module_exists('views_ui')) {
    // Hack to get consistent style with views ctools dropbutton.
    if (module_load_include('inc', 'views_ui', 'includes/admin')) {
      foreach (views_ui_get_admin_css() as $file => $options) {
        backdrop_add_css($file, $options);
      }
    }
  }

  $form = $variables['form'];

  if (!module_exists('page_actions')) {
    $form['actions']['#prefix'] = '<div id="module-filter-submit">';
    $form['actions']['#suffix'] = '</div>';
  }

  $header = array();
  $header[] = array('data' => '', 'class' => array('checkbox'));
  $header[] = array('data' => t('Name'), 'class' => array('name'));
  if ($config->get('version_column')) {
    $header[] = array('data' => t('Version'), 'class' => array('version'));
  }
  $header[] = array('data' => t('Description'), 'class' => array('description'));
  $header[] = array('data' => t('Links'), 'class' => array('links'));

  $package_ids = array('all');
  $enabled['all'] = array();

  if ($config->get('track_recent_modules')) {
    $recent_modules = array_filter($config->get('recent_modules'), 'module_filter_recent_filter');
    // Save the filtered results.
    $config->set('recent_modules', $recent_modules);

    $package_ids[] = 'recent';
    $enabled['recent'] = array();
  }

  // Determine what modules are new (within a week).
  $new_modules = module_filter_new_modules();
  $module_data = system_rebuild_module_data();
  $package_ids[] = 'new';
  $enabled['new'] = array();

  $rows = array();
  $flip = array('even' => 'odd', 'odd' => 'even');
  foreach (element_children($form['modules']) as $package) {
    $package_id = module_filter_get_id($package);
    $package_ids[] = $package_id;

    // Package title and header.
    $rows[] = array(
      'data' => array(
        array(
          'data' => '<h3>' . $form['modules'][$package]['#title'] . '</h3>',
          'colspan' => 4,
        ),
      ),
      'id' => $package_id . '-package',
      'class' => array('admin-package-title'),
    );
    $rows[] = array('data' => $header, 'class' => array('admin-package-header'));

    $stripe = 'odd';
    $enabled[$package_id] = array();
    foreach (element_children($form['modules'][$package]) as $key) {
      $module = &$form['modules'][$package][$key];

      $is_enabled = isset($module['enable']['#default_value']) ? $module['enable']['#default_value'] : '';
      $enabled['all'][] = $enabled[$package_id][] = $is_enabled;
      if (isset($recent_modules[$key])) {
        $enabled['recent'][] = $is_enabled;
      }
      if (isset($new_modules[$key])) {
        $enabled['new'][] = $is_enabled;
      }

      $row = array();

      $version = !empty($module['version']['#markup']);
      $version_column = $config->get('version_column');
      $show_path = $config->get('show_path');
      $requires = !empty($module['#requires']);
      $required_by = !empty($module['#required_by']);

      $toggle_enable = '';
      if (isset($module['enable']['#type']) && $module['enable']['#type'] == 'checkbox') {
        unset($module['enable']['#title']);
        $class = ($is_enabled ? 'enabled' : 'off');
        if (!empty($module['enable']['#disabled'])) {
          $class .= ' disabled';
        }
        $toggle_enable = '<div class="js-hide toggle-enable ' . $class . '"><div>&nbsp;</div></div>';
      }
      $row[] = array('class' => array('checkbox'), 'data' => $toggle_enable . backdrop_render($module['enable']));

      $label = '<label';
      if (isset($module['enable']['#id'])) {
        $label .= ' for="' . $module['enable']['#id'] . '"';
      }
      $row[] = array('class' => array('name'), 'data' => $label . '><strong>' . backdrop_render($module['name']) . '</strong> <span class="module-machine-name">(' . $key . ')</span></label>');

      if ($config->get('version_column')) {
        // Need something in the array slot, even if version is empty, otherwise
        // the rest of the cells are shifted to the left.
        $row[] = array('class' => array('version'), 'data' => backdrop_render($module['version']));
      }

      // Add the description, along with any modules it requires.
      if (empty($module['description']['#markup'])) {
        $module['description']['#markup'] = '<em>' . t('No description available.') . '</em>';
      }
      $description = '<span class="details"><span class="text">' . backdrop_render($module['description']) . '</span></span>';
      if (($version && !$version_column) || $show_path || $requires || $required_by) {
        $description .= '<div class="requirements">';
        if ($version && !$version_column) {
          $description .= '<div class="admin-requirements">' . t('Version: !module-version', array('!module-version' => backdrop_render($module['version']))) . '</div>';
        }
        if ($show_path) {
          $module_path = backdrop_get_path('module', $key);
          $description .= '<div class="admin-requirements path" title="' . $module_path . '">' . t('Path: !module-path', array('!module-path' => $module_path)) . '</div>';
        }

        // Display schema version for enabled modules.
        if (!empty($module_data[$key]->status) && !empty($module_data[$key]->schema_version)) {
          $description .= '<div class="admin-requirements">' . t('Schema version: @schema_version', array('@schema_version' => $module_data[$key]->schema_version)) . '</div>';
        }
        if ($requires) {
          $description .= '<div class="admin-requirements">' . t('Requires: !module-list', array('!module-list' => implode(', ', $module['#requires']))) . '</div>';
        }
        if ($required_by) {
          $description .= '<div class="admin-requirements">' . t('Required by: !module-list', array('!module-list' => implode(', ', $module['#required_by']))) . '</div>';
        }
        $description .= '</div>';
      }
      $row[] = array('data' => '<div class="inner expand" role="button">' . $description . '</div>', 'class' => array('description'));
      $row[] = array('data' => '<div class="links">' . backdrop_render($module['links']) . '</div>', 'class' => array('links'));

      $class = array(module_filter_get_id($package) . '-tab', 'module', $stripe);
      if (isset($recent_modules[$key])) {
        $class[] = 'recent-module';
      }
      if (isset($new_modules[$key])) {
        $class[] = 'new-module';
      }
      $rows[] = array('data' => $row, 'no_striping' => TRUE, 'class' => $class);
      $stripe = $flip[$stripe];
    }

    // Set the package as printed.
    $form['modules'][$package]['#printed'] = TRUE;
  }

  // Get packages and count number of modules.
  $enabled_counts = array();
  foreach ($enabled as $package_id => $value) {
    $enabled_counts[$package_id] = array(
      'enabled' => count(array_filter($value)),
      'total' => count($value),
    );
  }
  backdrop_add_js(array(
    'moduleFilter' => array(
      'packageIDs' => $package_ids,
      'enabledCounts' => $enabled_counts,
    ),
  ), 'setting');

  // Add first and last class to rows.
  $rows[0]['class'][] = 'first';
  $rows[count($rows) - 1]['class'][] = 'last';

  $output = '<div id="module-filter-wrapper">';
  $output .= '<div id="module-filter-modules">' . backdrop_render($form['module_filter']);
  $output .= theme('table__module_filter_system_modules_tabs', array('header' => $header, 'rows' => $rows));
  $output .= backdrop_render_children($form);
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}

