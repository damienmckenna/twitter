<?php
/**
 * @file
 * Views handlers for Twitter module.
 */

/**
 * Process Twitter-style @usernames and URLs before filtering XSS.
 */
class twitter_views_handler_field_xss extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['link_urls'] = array('default' => TRUE);
    $options['link_usernames'] = array('default' => TRUE);
    $options['link_hashtags'] = array('default' => TRUE);
    $options['link_attributes'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_urls'] = array(
      '#title' => t('Link urls to their destinations'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_urls']),
    );
    $form['link_usernames'] = array(
      '#title' => t('Link Twitter @usernames to their Twitter.com urls'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_usernames']),
    );
    $form['link_hashtags'] = array(
      '#title' => t('Link Twitter #hashtags to another url'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_hashtags']),
    );
    $form['link_attributes'] = array(
      '#title' => t('Open links in new windows/tabs and add rel="nofollow" attributes.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_attributes']),
    );
  }

  /**
   * Processes the message through the selected options.
   */
  function render($values) {
    $value = $values->{$this->field_alias};

    // Load the options.
    $usernames = TRUE;
    if (isset($this->options['link_usernames'])) {
      $usernames = $this->options['link_usernames'];
    }
    $hashtags = TRUE;
    if (isset($this->options['link_hashtags'])) {
      $hashtags = $this->options['link_hashtags'];
    }
    $attributes = TRUE;
    if (isset($this->options['link_attributes'])) {
      $attributes = $this->options['link_attributes'];
    }
    $urls = TRUE;
    if (isset($this->options['link_urls'])) {
      $urls = $this->options['link_urls'];
    }

    // Offload processing to the shared function.
    return twitter_filter_message($value, $usernames, $hashtags, $attributes, $urls);
  }
}

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 */
class twitter_views_handler_field_profile_image extends views_handler_field {
  function render($values) {
    $path = $values->{$this->field_alias};
    $output = theme('image', $path, '', '', array('width' => 48, 'height' => '48'), FALSE);

    // Convert to a protocol-relative URL so that the same image tag will work
    // regardless of whether the page is loaded via HTTP or HTTPS.
    return str_replace('http:', '', $output);
  }
}
